{
  description = "A flake for building and running snake";

  inputs.nixpkgs.url = github:NixOS/nixpkgs/nixos-unstable;

  outputs = { self, nixpkgs }:
    let
      lib = nixpkgs.lib;
      supportedSystems = ["x86_64-linux" "i686-linux" "aarch64-linux"];
      makePackage = sys: 
      (with import nixpkgs { system = sys; }; 
      stdenv.mkDerivation {
        name = "snake";
        src = self;
        buildInputs = [pkgs.gcc pkgs.coreutils pkgs.ncurses];
        buildPhase = "gcc -lncurses -o snake snake.c";
        installPhase = "mkdir -p $out/bin; install -t $out/bin snake";
      });
      makeApp = sys: {
        type = "app";
        program = "${self.defaultPackage.${sys}}/bin/snake";
      };
    in {
      defaultPackage = lib.attrsets.genAttrs supportedSystems makePackage;
      defaultApp = lib.attrsets.genAttrs supportedSystems makeApp;
    };
}
