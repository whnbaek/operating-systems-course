import gdb

class TreePrinter(gdb.Command):
    """Pretty print the tree_t structure."""

    def __init__(self):
        super(TreePrinter, self).__init__("print-tree", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        # Parse the argument as a pointer to tree_t
        root = gdb.parse_and_eval(arg)
        self.pretty_print_tree(root, 0)

    def pretty_print_tree(self, node, level):
        if node == 0:
            return

        # Right subtree
        self.pretty_print_tree(node['right'], level + 1)

        # Current node
        print(" " * (4 * level) + f"{node['val']}")

        # Left subtree
        self.pretty_print_tree(node['left'], level + 1)

TreePrinter()

