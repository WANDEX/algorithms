#!/usr/bin/env python3
# coding=utf-8

__license__ = "0BSD"
__description__ = "Script for embedding specific fs tree into README with relative markdown urls."
__url_repository__ = "https://github.com/WANDEX/algorithms"
__url_project__ = __url_repository__
__url_repository_original__ = "https://github.com/WANDEX/algorithms"
__author__ = "WANDEX"

# TODO extract all inside /\*\* \*/
# TODO remove leading "^ * "
# TODO remove leading & trailing whitespaces from each line
# TODO create dict fname[file_description]
# TODO output mp_file_url & description into raw txt file
# TODO compare new txt file with old if files differ => update README
# TODO insert text into README between <details> </details>

import FSTreeDisplay

import pprint
import subprocess

from collections import OrderedDict
from filecmp import cmp
from pathlib import Path
from shutil import copy2, which


# Global Constants - for easier adoption in other projects
TXT_DIR = Path("./scripts/.cache/raw/")
TXT_NEW = TXT_DIR.joinpath("tree.new.txt")
TXT_OLD = TXT_DIR.joinpath("tree.old.txt")
README = Path("./README.md")
TESTS_DIR = Path("./tests/")
INCLUDE_DIR = Path("./include/")


def ppr(arg):
    """pprint()"""
    pprint.PrettyPrinter().pprint(arg)


def at_path(executable) -> bool:
    """return True if executable is found at PATH."""
    if which(executable):
        return True
    return False


def get_grep_cmd() -> list:
    if at_path("rg"):
        return "rg".split()
    if at_path("grep"):
        return "grep -r".split()
    raise Exception("grep/rg executable not found at path!")


def grep(pattern: str, path) -> str:
    """grep path for pattern."""
    cmd = get_grep_cmd()
    cmd.append(pattern)
    cmd.append(str(path))
    p = subprocess.run(cmd, text=True, stdout=subprocess.PIPE)
    out = str(p.stdout).strip()
    if p.returncode == 1:  # rg/grep(1) => pattern not found
        return ""  # -> return empty string
    elif p.returncode != 0:
        raise Exception(f"grep/rg cmd ERROR({p.returncode})\n{p.stderr}")
    return out


def grep_bool(pattern: str, path) -> bool:
    if not grep(pattern, path):
        return False
    return True


def filter_paths(paths: list, globs: tuple) -> list:
    """Filter/Exclude from paths all matches of pattern glob."""
    remove_those = []
    for path in paths:
        for g in globs:
            if path.match(g):
                remove_those.append(path)
                break
    for r in remove_those:
        paths.remove(r)
    return paths


def list_tests() -> list:
    """Construct list of file paths of the tests dir."""
    tests = [f for f in TESTS_DIR.rglob("*") if f.is_file()]
    tests = filter_paths(tests, ("*.txt", "*main.cc"))
    return tests


def find_respective_test_files(fpath: Path) -> list:
    """Find test files for the file and return as list of paths."""
    resp_tests = []
    tests_fpaths = list_tests()
    for test_fpath in tests_fpaths:
        if grep_bool(f"#include .*{fpath.name}", test_fpath):
            resp_tests.append(test_fpath)
    return resp_tests


def make_md_url(fpath: Path) -> str:
    """Make relative url of file path to embed into markdown file."""
    return f"[{fpath.name}](./{fpath})"


def make_md_url_and_find_tests(fpath: Path) -> str:
    """Make & add md urls of the test files if any."""
    hurl = make_md_url(fpath)
    resp_tests = find_respective_test_files(fpath)
    test_files_urls = []
    if resp_tests:  # if not empty (found respective test files)
        for f in resp_tests:
            test_files_urls.append(make_md_url(f))
        return f"{hurl} {test_files_urls[:]}"
    return f"{hurl}"  # only file url (test file not found)


def make_md_tree(rws=True) -> OrderedDict:
    """Make specific fs tree for embedding into markdown."""
    # TODO: criteria to check if path is under git version control
    FSTD = FSTreeDisplay

    #  def mdcriteria(ipath: str):
    #      #  if FSTD.is_empty(ipath):
    #      #      return False
    #      return sane_criteria(ipath)

    paths = FSTD.FSTreeDisplay.make_tree(INCLUDE_DIR, criteria=FSTD.sane_criteria)
    od_tree = OrderedDict()
    for path in paths:
        if rws:  # replace whitespaces: ws -> En Space
            od_tree[path.paths] = path.displayable().replace(' ', ' ') + '\\'
        else:
            od_tree[path.paths] = path.displayable() + '\\'
    return od_tree


def md_tree_lines() -> list:
    """Specifically process OrderedDict of fs tree for embedding into markdown."""
    od_tree = make_md_tree()

    def is_file(fpath: Path) -> bool:
        return fpath.is_file()

    def replace_fname(fpath: Path, s: str):
        od_tree[fpath] = str(s).replace(fpath.name, make_md_url_and_find_tests(fpath))

    for (k_fpath, v_str) in od_tree.items():
        if not is_file(k_fpath):
            continue
        replace_fname(k_fpath, v_str)

    lines = []
    for v_str in od_tree.values():
        lines.append(v_str)
    # remove last \\ character from the last line
    lines[-1] = str(lines[-1]).removesuffix('\\')
    return lines


def write_tree(lines: list) -> bool:
    """Write lines of the markdown tree into file."""
    TXT_DIR.mkdir(parents=True, exist_ok=True)
    if TXT_NEW.is_file():
        copy2(TXT_NEW, TXT_OLD)
    else:
        TXT_NEW.touch()
        TXT_OLD.touch()
    with open(TXT_NEW, 'w', encoding='utf-8') as f:
        f.write('\n'.join(lines))
    # -> True (files have equal content)
    return cmp(TXT_NEW, TXT_OLD, shallow=False)


def embed_tree():
    pass


def main():
    lines = md_tree_lines()

    # XXX for ease of viewing result in terminal
    for line in lines:
        print(line)

    # XXX uncomment!
    is_equal = write_tree(lines)
    if not is_equal:
        print("files are not equal!")


if __name__ == "__main__":
    main()
