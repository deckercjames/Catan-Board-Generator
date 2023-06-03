
# Settlers of Catan Board Generator

## Overview
This is a script to generate random PDF boards for Settlers of Catan.

The latex files were created by u/likethevegetable:
https://www.reddit.com/r/Ultralight/comments/itd5n0/ultralight_settlers_of_catan_project_catikz/
https://www.overleaf.com/read/xbvtgbrsmgpb

The c code simply generates random stimulus csv files for latex to use.

## How to run
The `generate.sh` script can be run and it will create a pdf of two pages (one board per page) and store it in the `output/` directory. Any existing `Board.pdf` file will be overwritten. The working directory should be the top level of the repo, where the generate script is.