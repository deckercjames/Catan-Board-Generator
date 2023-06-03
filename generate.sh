

# Compile the c code
echo "Compiling..."
mkdir bin
gcc ./src/SoC.c -o ./bin/SoC.out

# Check that compile exited succesfully
if [ $? -ne 0 ]; then
    echo "Failed to Compile. Aborting."
    exit 1
fi

# Generate two source files for the pdf generator
./bin/SoC.out $RANDOM > ./CaTikZ/Example1.csv
./bin/SoC.out $RANDOM > ./CaTikZ/Example2.csv

# Check that program correctly wrote the files
if [ $? -ne 0 ]; then
    echo "Failed to generate board stimulus. Aborting."
    exit 1
fi

# Check that pdflatex is installed
if ! command -v pdflatex &> /dev/null
then
    echo "The command 'pdflatex' is required for this script to generate the PDFs."
    exit 1
fi

# Generate the board pdf with latex
cd CaTikZ/
pdflatex CaTikZ.tex
cd ..

# Make a directory to store the output
mkdir output

# Move the generated files to the ouput folder
mv CaTikZ/CaTikZ.pdf output/Board.pdf
mv CaTikZ/ResourceCards.pdf output/ResourceCards.pdf

echo "Done"