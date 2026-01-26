{
  description = "CineMania";

  inputs = {
    nixpkgs.url = "https://channels.nixos.org/nixpkgs-unstable/nixexprs.tar.xz";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        packages.default = pkgs.stdenv.mkDerivation {
          pname = "cinemania";
          version = "1.0.0";

          src = ./.;

          buildInputs = [ ];

          nativeBuildInputs = [ pkgs.gcc ];

          buildPhase = ''
            make
          '';

          installPhase = ''
            mkdir -p $out/bin
            cp cinemania $out/bin/
          '';

          meta = with pkgs.lib; {
            description = "Terminal-based movie management system";
            license = licenses.mit;
            platforms = platforms.unix;
          };
        };

        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            gcc
            gnumake
          ];

          shellHook = ''
            echo "Available commands:"
            echo "  make       - Build the project"
            echo "  make clean - Clean build files"
            echo "  ./cinemania - Run the program"
          '';
        };

        apps.default = {
          type = "app";
          program = "${self.packages.${system}.default}/bin/cinemania";
        };
      }
    );
}
