#!/usr/bin/env python3
# coding=utf-8

from pathlib import Path
from sys import argv


class FSTreeDisplay(object):
    display_fs_prefix_middle = '├──'
    display_fs_prefix_last = '└──'
    display_parent_prefix_middle = '    '
    display_parent_prefix_last = '│   '

    def __init__(self, path, parent_path, is_last):
        self.path = Path(str(path))
        self.parent = parent_path
        self.is_last = is_last
        if self.parent:
            self.depth = self.parent.depth + 1
        else:
            self.depth = 0

    @property
    def is_dir(self):
        return self.path.is_dir()

    @property
    def paths(self):
        return self.path

    @property
    def name(self):
        return self.path.name

    @property
    def displayname(self):
        if self.path.is_dir():
            return self.path.name + '/'
        return self.path.name

    @classmethod
    def make_tree(cls, root, parent=None, is_last=False, criteria=None):
        root = Path(str(root))
        criteria = criteria or cls._default_criteria

        displayable_root = cls(root, parent, is_last)
        yield displayable_root

        children = sorted(list(path
                          for path in root.iterdir()
                          if criteria(path)),
                          key=lambda s: str(s).lower())
        count = 1
        for path in children:
            is_last = count == len(children)
            if path.is_dir():
                yield from cls.make_tree(path,
                                         parent=displayable_root,
                                         is_last=is_last,
                                         criteria=criteria)
            else:
                yield cls(path, displayable_root, is_last)
            count += 1

    @classmethod
    def is_hidden(cls, path):
        return not path.name.startswith(".")

    # TODO: criteria to check if path is under git version control
    #  @classmethod
    #  """Is path under git version control."""
    #  def under_gvc(cls, path):

    # XXX THIS IS NOT WORKING FOR DIRS!
    #  @classmethod
    #  def is_empty(cls, path):
    #      """Check if path dir/file is empty."""
    #      return not path.stat().st_size

    @classmethod
    def exclude(cls, path, exclude_paths):
        if path.name in exclude_paths:
            return True
        return False

    @classmethod
    def exclude_ending_with(cls, path, exclude_path_ends):
        if path.name.endswith(exclude_path_ends):
            return True
        return False

    @classmethod
    def _default_criteria(cls, path):
        # never show hidden with default criteria
        if not cls.is_hidden(path):
            return False
        return True

    def displayable(self):
        if self.parent is None:
            return self.displayname

        _fs_prefix = (self.display_fs_prefix_last
                      if self.is_last
                      else self.display_fs_prefix_middle)

        parts = ['{!s} {!s}'.format(_fs_prefix, self.displayname)]

        parent = self.parent
        while parent and parent.parent is not None:
            parts.append(self.display_parent_prefix_middle
                         if parent.is_last
                         else self.display_parent_prefix_last)
            parent = parent.parent

        return ''.join(reversed(parts))


def sane_criteria(ipath: str):
    """Exclude specific paths."""
    FSTD = FSTreeDisplay
    if not FSTD.is_hidden(ipath):
        return False
    if FSTD.exclude(ipath, (".git", "__pycache__")):
        return False
    if FSTD.exclude_ending_with(ipath, (".cache", ".txt")):
        return False
    return True


def print_fs_tree(ipath: str):
    """Print fs path as tree structure."""
    paths = FSTreeDisplay.make_tree(ipath, criteria=sane_criteria)
    for path in paths:
        print(path.displayable())


if __name__ == "__main__":
    arg = "."  # current dir if no arg
    if len(argv[:]) > 1:
        arg = str(argv[1])  # supply path as arg
    print_fs_tree(arg)
