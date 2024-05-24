{ pkgs }: {
	deps = [
   pkgs.nano
   pkgs.tree
		pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
	];
}