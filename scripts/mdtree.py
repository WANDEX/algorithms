#!/usr/bin/env python3
# coding=utf-8

__license__ = "0BSD"
__description__ = "Embed specific fs tree into markdown using relative links."
__url_repository__ = "https://github.com/WANDEX/algorithms"
__url_project__ = __url_repository__
__url_repository_original__ = "https://github.com/WANDEX/algorithms"
__author__ = "WANDEX"

import FSTreeDisplay

import re
import subprocess

from collections import OrderedDict
from pathlib import Path
from pprint import PrettyPrinter
from shutil import copy2, which
from sys import argv, stdout

TPRINT = False

# jump to the 'md_criteria' to exclude certain paths from the tree
# Global Constants - for easier adoption in other projects
INCLUDE_DIR = Path("./include/")
BACKUP_DIR = Path("./scripts/.cache/md_backup/")
TESTS_DIR = Path("./tests/")
README = Path("./README.md")
README_BACKUP = BACKUP_DIR.joinpath("README.md")

# regex to select everything between this two headers
README_MULTILINE_RE = r'\
^## Tree of Implemented DSA$\
(.*)\
^## Hall of Fame$'


def ppr(arg):
    """pprint() - only for more readable visualization during debugging/developing."""
    PrettyPrinter().pprint(arg)


def at_path(executable) -> bool:
    """return True if executable is found at $PATH."""
    if which(executable):
        return True
    return False


def get_grep_cmd() -> list:
    """Use specific grep command based on availability of executable at $PATH."""
    if at_path("rg"):
        return "rg".split()
    if at_path("grep"):
        return "grep -r".split()
    raise Exception("grep/rg executable not found at path!")


def grep(pattern: str, path: Path | str) -> str:
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


def grep_bool(pattern: str, path: Path | str) -> bool:
    """grep & return only bool result of operation."""
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


def make_md_link_rel(fpath: Path) -> str:
    """Make relative link of file path."""
    return f"[{fpath.name}](./{fpath})"


def make_md_link_var(fpath: Path) -> tuple[str, str]:
    """Make relative link & link variable definition of file path."""
    return f"[{fpath.name}]", f"[{fpath.name:<28}]: ./{fpath}"


def make_md_link_and_find_tests(fpath: Path) -> tuple[str, set]:
    """Make & add md links & test files if any."""
    link_defs = set()

    link_name, link_def = make_md_link_var(fpath)
    link_defs.add(link_def)

    resp_tests = find_respective_test_files(fpath)
    if resp_tests:  # if not empty (found respective test files)
        lnt = set()
        for test_fpath in resp_tests:
            tlink_name, tlink_def = make_md_link_var(test_fpath)
            lnt.add(tlink_name)
            link_defs.add(tlink_def)
        # specific string format to make less noisy visually (more pleasant)
        tests_lnames = str(sorted(lnt))\
            .replace("['", '( *').replace("']", '* )').replace("', '", ' | ')
        return f"{link_name} {tests_lnames}", link_defs
    return link_name, link_defs  # only file url (test file not found)


def make_md_tree(rws=True) -> OrderedDict:
    """Make specific fs tree for embedding into markdown."""
    FSTD = FSTreeDisplay

    def md_criteria(ipath: str):
        if not FSTD.sane_criteria(ipath):
            return False
        if FSTD.FSTreeDisplay.exclude(ipath, ("common", "__pycache__", "c")):
            return False
        if not FSTD.FSTreeDisplay.under_gvc(ipath):
            return False
        return True

    paths = FSTD.FSTreeDisplay.make_tree(INCLUDE_DIR, criteria=md_criteria)
    od_tree = OrderedDict()
    for path in paths:
        if rws:  # replace whitespaces: ws -> En Space
            od_tree[path.paths] = path.displayable().replace(' ', ' ') + '\\'
        else:
            od_tree[path.paths] = path.displayable() + '\\'
    return od_tree


def gen_md_tree_lines() -> list:
    """Specifically process OrderedDict of fs tree for embedding into markdown."""
    od_tree = make_md_tree()
    link_defs = set()

    def is_file(fpath: Path) -> bool:
        return fpath.is_file()

    def replace_fname(fpath: Path, s: str):
        link_name, ldefs = make_md_link_and_find_tests(fpath)
        link_defs.update(ldefs)
        od_tree[fpath] = str(s).replace(fpath.name, link_name)

    for (k_fpath, v_str) in od_tree.items():
        if not is_file(k_fpath):
            continue  # skip
        replace_fname(k_fpath, v_str)

    lines = []
    for v_str in od_tree.values():
        if TPRINT:
            print(v_str)
        lines.append(v_str)

    # replace last \\ character from the last line
    # to insert proper empty line between tree & link definitions
    lines[-1] = str(lines[-1]).replace('\\', '\n')

    # append sorted list of unique link definitions
    sorted_link_defs = sorted(link_defs)
    #  for link_def in link_defs:
    for link_def in sorted_link_defs:
        lines.append(link_def)

    return lines


def fread_to_str(fpath: Path | str) -> str:
    """Read file & return its contents as string."""
    fcontent = ""
    with open(fpath, 'r', encoding='utf-8') as f:
        fcontent = f.read()
    return fcontent


def freplace(fpath: Path | str, old: str, new: str):
    """Replace old by the new string in the file."""
    old_file_content = fread_to_str(fpath)
    new_file_content = old_file_content.replace(old, new)
    with open(fpath, 'w', encoding='utf-8') as f:
        f.write(new_file_content)


def extract_md_tree() -> str:
    """Extract tree from markdown by regex for later replacement."""
    fcontent = fread_to_str(README)
    res = re.search(README_MULTILINE_RE, fcontent, flags=re.DOTALL | re.MULTILINE)
    if not res:
        raise re.error("Nothing was found by the provided regex.")
    return res.group(1)


def cmp_and_upd() -> int:
    """Compare tree with tree in md, and update tree if trees are different."""
    # generate and specifically process fs tree for the markdown file
    gen = gen_md_tree_lines()
    old = extract_md_tree()
    new = '\n'.join(gen) + '\n'
    if old == new:
        return 2  # -> success => no need to replace tree in md

    # make backup before making file modifications
    BACKUP_DIR.mkdir(parents=True, exist_ok=True)
    copy2(README, README_BACKUP)
    # replace old tree by the new one
    freplace(README, old, new)

    # double check after replacing
    updated = extract_md_tree()
    if new == updated:
        return 1  # -> success

    # use backup file to revert md to state before replacement
    copy2(README_BACKUP, README)
    return 3  # -> fail but backup were used! (state of md file should be reverted)


def output_is_piped() -> bool:
    """Determine if script output were piped."""
    return not stdout.isatty()


def ccons():
    """Constants for the colored terminal text.
    Enable colors only if script output were not piped.
    (moved into separate function to deliberately use colors very little)
    """
    RED = ''
    GRN = ''
    CYN = ''
    END = ''
    # allow to use ASCII escape sequences of colors
    if not output_is_piped():
        RED = '\033[1;31m'
        GRN = '\033[1;32m'
        CYN = '\033[1;36m'
        END = '\033[0m'
    return RED, GRN, CYN, END


def main():
    RED, GRN, CYN, END = ccons()
    ret = cmp_and_upd()
    if ret == 3:
        print(f"{RED}Something gone wrong!\nAs a result the md file was restored from a backup!{END}")
        exit(3)
    elif ret == 1:
        print(f"{GRN}Updated tree was successfully embedded! 1337{END}")
    elif ret == 2:
        print(f"{CYN}New tree is equal to the old => no need in update of markdown file.{END}")


if __name__ == "__main__":
    if len(argv[:]) > 1:
        arg = str(argv[1])
        if 'p' in arg:
            TPRINT = True
    main()
