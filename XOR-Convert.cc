/*
XOR-Convert
Two inputfiles (called: input_file, mask_file) are converted into the outputfile

The XOR operation is applied to the n-th byte of each inputfile and
written to the n-th byte of the outputfile. It should be noted that the
mask_file must be equal or larger than the input_file.

the variable buffer_max can be adjusted depending on the amount of free memory.
*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

size_t buffer_max = 10000000, readnumberofbytes;
size_t length_file1, length_file2;
ifstream inputfile, maskfile;
ofstream outputfile;

void openfiles(string filename, string maskname) {
  inputfile.open(filename);
  maskfile.open(maskname);
  if (!inputfile || !maskfile) {
    cout << "Error: missing file(s)" << endl;
    if (!inputfile) cout << filename << " ";
    if (!maskfile) cout << maskname;
    cout << endl;
    exit(0);
  }

  inputfile.seekg(0, inputfile.end);
  length_file1 = inputfile.tellg();
  inputfile.seekg(0, inputfile.beg);

  maskfile.seekg(0, maskfile.end);
  length_file2 = maskfile.tellg();
  maskfile.seekg(0, maskfile.beg);
}

void inputfileF(vector<char>* buffer) {
  if (length_file1 < buffer_max) {
    buffer->resize(length_file1);
    inputfile.read(&buffer->at(0), length_file1);
  } else {
    buffer->resize(readnumberofbytes);
    inputfile.read(&buffer->at(0), readnumberofbytes);
  }
}

void maskfileF(vector<char>* buffer) {
  if (length_file1 < buffer_max) {
    buffer->resize(length_file1);
    maskfile.read(&buffer->at(0), length_file1);
  } else {
    buffer->resize(readnumberofbytes);
    maskfile.read(&buffer->at(0), readnumberofbytes);
  }
}

void writefile(vector<char>* buffer) {
  if (outputfile) {
    outputfile.write(&buffer->at(0), readnumberofbytes);
  }
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    cout << "Error: missing file operand(s)" << endl
         << "XOR-Convert input_file mask_file result_file" << endl;
    exit(0);
  }

  string filename, maskname, outputname;
  vector<char> file_buf, mask_buf, output_buf;
  int cycles;

  filename = argv[1];
  maskname = argv[2];
  outputname = argv[3];

  openfiles(filename, maskname);

  if (length_file1 == 0 || length_file2 == 0) {
    cout << "Error: " << filename << " or " << maskname << " are empty !"
         << endl;
    exit(0);
  }

  if (length_file1 > length_file2) {
    cout << "Error: " << filename << " is bigger than " << maskname << endl;
    exit(0);
  }

  outputfile.open(outputname);

  cycles = length_file1 / buffer_max;
  readnumberofbytes = buffer_max;

  for (int i = 0; i < cycles + 1; i++) {
    if (i == cycles) {
      readnumberofbytes = length_file1 % buffer_max;
    }

    inputfileF(&file_buf);
    maskfileF(&mask_buf);
    output_buf.resize(readnumberofbytes);

    for (int i = 0; i < readnumberofbytes; i++) {
      output_buf[i] = (char)file_buf[i] ^ mask_buf[i];
    }

    writefile(&output_buf);
  }
}
