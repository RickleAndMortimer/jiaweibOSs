# shell.nix
{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    nativeBuildInputs = with pkgs.buildPackages; [ gpp gnumake xorriso qemu nasm ];
}
