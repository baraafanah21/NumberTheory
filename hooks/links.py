"""Adapt links written for GitHub browsing so they also work on the built site.

The Markdown is written to be read on GitHub, where two kinds of relative link
behave differently from a static site:

* A link to a folder (``../gcd/``) opens that folder's README on GitHub. On the
  site it is rewritten to point at the README page itself.
* A link to a file that is not a page (``implementation.cpp#L20``, ``LICENSE``)
  opens GitHub's file viewer, with line anchors. On the site it is rewritten to
  the same file on GitHub.

The source files are never modified; this only changes what the site renders.
"""

import os
import posixpath
import re

# Folders that have no README of their own, and the page that stands in for one.
FOLDER_PAGES = {
    "concepts": "README.md#concepts",
}

LINK = re.compile(r"(\]\()([^)\s]+)((?:\s+\"[^\"]*\")?\))")
FENCE = re.compile(r"^\s*(`{3,}|~{3,})")
CODE_SPAN = re.compile(r"(`+[^`]*`+)")


def on_page_markdown(markdown, page, config, files):
    page_dir = posixpath.dirname(page.file.src_uri)
    docs_dir = config["docs_dir"]
    blob_url = config["repo_url"].rstrip("/") + "/blob/main/"

    def rewrite(target):
        if re.match(r"^[a-z][a-z0-9+.-]*:|^[#/]", target, re.IGNORECASE):
            return target
        path, _, anchor = target.partition("#")
        resolved = posixpath.normpath(posixpath.join(page_dir, path))
        on_disk = os.path.join(docs_dir, resolved)

        if os.path.isdir(on_disk):
            if os.path.isfile(os.path.join(on_disk, "README.md")):
                new = posixpath.join(resolved, "README.md")
            elif resolved in FOLDER_PAGES:
                new, _, anchor = FOLDER_PAGES[resolved].partition("#")
            else:
                return target
            new = posixpath.relpath(new, page_dir or ".")
            return new + ("#" + anchor if anchor else "")

        file = files.get_file_from_path(resolved)
        if os.path.isfile(on_disk) and (file is None or not file.inclusion.is_included()):
            return blob_url + resolved + ("#" + anchor if anchor else "")

        return target

    def rewrite_line(line):
        parts = CODE_SPAN.split(line)
        for i in range(0, len(parts), 2):
            parts[i] = LINK.sub(lambda m: m.group(1) + rewrite(m.group(2)) + m.group(3), parts[i])
        return "".join(parts)

    out = []
    fence = None
    for line in markdown.split("\n"):
        match = FENCE.match(line)
        if fence:
            closing = line.strip()
            if closing and set(closing) == {fence[0]} and len(closing) >= len(fence):
                fence = None
            out.append(line)
        elif match:
            fence = match.group(1)
            out.append(line)
        else:
            out.append(rewrite_line(line))
    return "\n".join(out)
