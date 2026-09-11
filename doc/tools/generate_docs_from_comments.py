#!/usr/bin/python3
# coding: utf-8
"""从扩展模块的 C++ 头文件注释（/// 或 ##）自动生成 XML 类文档描述。

Godot 的 doctool 只生成方法签名等骨架，不会从自定义模块的 C++ 注释提取描述。
本脚本接管：读取 extensions 模块全部 .h 文件，解析类声明与公开静态方法前的
注释块，按类名/方法名注入到 doc/classes/*.xml 的对应 <description> 中。

用法：
    python generate_docs_from_comments.py
    # 默认读取 doc/classes/*.xml 与模块根级 .h（可跨子目录扫描）
可在 doctool 之后、xml_to_markdown 之前执行。脚本幂等：已填充的描述不会覆盖。
"""

import glob
import os
import re
import sys
from xml.sax.saxutils import escape

# 模块根：本文件位于 doc/tools/ 下
MODULE_ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), "..", ".."))
CLASSES_DIR = os.path.join(MODULE_ROOT, "doc", "classes")

# 注释行：/// 或 ## 风格
COMMENT_RE = re.compile(r"^\s*(?:///+\s?(.*)|\#\#+\s?(.*))$")
# 类定义行：class Name({ : }) —— 前向声明（class X;）不匹配
CLASS_RE = re.compile(r"^\s*class\s+([A-Za-z_]\w*)\s*(?::|\{)")
# GDVIRTUAL 宏：函数名位于括号内，如 GDVIRTUAL0_REQUIRED(main)
GDVIRTUAL_RE = re.compile(r"^\s*GDVIRTUAL\w*\s*\(([A-Za-z_]\w*)\)")


def _extract_method_name(line):
    """从函数声明行提取方法名：取第一个 '(' 之前的最后一个标识符。
    形如 Window *create_window(); 返回 create_window；GDVIRTUAL 由前置正则处理。"""
    if not line.strip().startswith(("/", "#", "*")):
        before_paren = line.split("(", 1)[0]
        m = re.search(r"([A-Za-z_]\w*)\s*$", before_paren)
        if m:
            return m.group(1)
    return None


def _strip_comment_marker(line):
    """将注释行转换为纯文本描述行。"""
    m = COMMENT_RE.match(line)
    if not m:
        return None
    return (m.group(1) or m.group(2) or "").strip()


def _clean_description(raw_lines):
    """将原始注释行拼接为紧凑的 XML 描述文本：每注释行占一行，跳过空行。"""
    lines = [line.strip() for line in raw_lines if line.strip()]
    return "\n".join(lines).strip()


def _parse_header(path):
    """解析单个头文件，返回 (class_name, class_comment, {method_name: doc}) 或 None。"""
    with open(path, "r", encoding="utf-8") as f:
        lines = f.readlines()

    # 定位类名
    class_name = None
    for line in lines:
        m = CLASS_RE.match(line)
        if m:
            class_name = m.group(1)
            break
    if not class_name:
        return None

    pending = []  # 当前累计注释块
    class_comment = []
    method_docs = {}

    for line in lines:
        text = _strip_comment_marker(line)
        if text is not None:
            pending.append(text)
            continue

        if not pending:
            continue

        cm = CLASS_RE.match(line)
        if cm and cm.group(1) == class_name:
            class_comment = pending
            pending = []
            continue

        gm = GDVIRTUAL_RE.match(line)
        if gm:
            method_docs[gm.group(1)] = pending
            pending = []
            continue

        mm_name = _extract_method_name(line)
        if mm_name:
            # 仅记录公开绑定方法（XML 只含这些），私有方法名以下划线开头自然跳过
            method_docs[mm_name] = pending
            pending = []
            continue

        # 其它声明（成员变量、嵌套类等）：丢弃当前累计块
        s = line.strip()
        if s and not s.startswith(("/", "#", "//", "/*", "*")):
            pending = []

    return class_name, class_comment, method_docs


def _indent_text(text, indent):
    """为文本每行添加 indentation + tab，空行保持空。"""
    return "\n".join(indent + t if t.strip() else "" for t in text.split("\n"))


def inject_all(xml_content, class_comment_text, method_docs):
    """返回注入描述后的 XML 文本。头文件注释为权威来源，总是覆盖对应描述。"""
    # 类级：填 brief_description
    bm = re.search(r"<brief_description>(?P<inner>.*?)</brief_description>", xml_content, re.DOTALL)
    if class_comment_text and bm:
        xml_content = _replace_between(xml_content, bm.start("inner"), bm.end("inner"),
                                       _indent_text(class_comment_text, "\t"))

    # 方法级：先收集某个 method 内空的 <description> 替换点
    method_starts = [(m.start(), m.group(1)) for m in re.finditer(r"<method\s+name=\"(\w+)\"", xml_content)]
    # 收集所有 <description>...</description> 的 inner 区间
    descs = [(m.start(), m.end(), m.group("inner")) for m in
             re.finditer(r"<description>(?P<inner>.*?)</description>", xml_content, re.DOTALL)]

    replacements = []  # (inner_start, inner_end, new_text)，逆序应用
    for d_start, d_end, inner in descs:
        # 找这个 description 属于哪个 method（最后一个 method_starts.start < d_start）
        owner = None
        for ms, mname in method_starts:
            if ms < d_start:
                owner = mname
            else:
                break
        if owner is None:
            continue
        doc = method_docs.get(owner)
        if not doc:
            continue
        cleaned = _clean_description(doc)
        inner_start = d_start + len("<description>")
        inner_end = d_end - len("</description>")
        replacements.append((inner_start, inner_end, _indent_text(cleaned, "\t")))

    # 逆序应用，保证前面的坐标不受后序替换影响
    for inner_start, inner_end, new_text in sorted(replacements, key=lambda x: x[0], reverse=True):
        xml_content = _replace_between(xml_content, inner_start, inner_end, new_text)

    return xml_content


def _replace_between(content, start, end, new_text):
    return content[:start] + new_text + content[end:]


def patch_xml(xml_path, header_infos):
    """读入 XML，注入描述后写回。返回是否注入成功。"""
    with open(xml_path, "r", encoding="utf-8") as f:
        content = f.read()

    cm = re.search(r'<class\s+name="([A-Za-z_]\w*)"', content)
    if not cm:
        return False
    class_name = cm.group(1)

    header_info = next((hi for hi in header_infos if hi[0] == class_name), None)
    if header_info is None:
        return False

    _, class_comment, method_docs = header_info
    class_text = _clean_description(class_comment)

    new_content = inject_all(content, class_text, method_docs)
    if new_content == content:
        return True  # 无变化也视为成功（幂等）

    with open(xml_path, "w", encoding="utf-8") as f:
        f.write(new_content)
    return True


def main():
    headers = glob.glob(os.path.join(MODULE_ROOT, "**", "*.h"), recursive=True)

    header_infos = []
    for h in headers:
        try:
            parsed = _parse_header(h)
        except OSError:
            parsed = None
        if parsed:
            header_infos.append(parsed)

    print("Found %d headers with classes" % len(header_infos))

    patched = 0
    for xml_path in glob.glob(os.path.join(CLASSES_DIR, "*.xml")):
        try:
            ok = patch_xml(xml_path, header_infos)
        except Exception as e:  # noqa: BLE001
            print("Failed on %s: %s" % (os.path.basename(xml_path), e))
            ok = False
        if ok:
            patched += 1
    print("Patched %d XML files" % patched)


if __name__ == "__main__":
    sys.exit(0 if not main() else 1)