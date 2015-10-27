#Advanced Encryption Standard

## About

Implementation of AES. Rijndael was selected by NIST (National Institute of Standards and Technology) as AES. AES is a symmetric cipher. Unlike Rijndael in which block length and key length can be specified to any multiple of 32 bits, AES fixes block length of 128 bits and key length to 128 bits, 192 bits or 256 bits.


## Usage
```bash
# make 
# ./aes input.txt key.txt
```

## Instructions 

- aes.cpp contains the actual code
- aes.h contains the header files required for aes.cpp 
- input.txt contains the ASCII message to be encrypted 
- key.txt contains the ASCII key used for encryption 
- output.txt contains the cipher along with output of each stage
- error.txt contains logs for intermediate steps

## Limitation

- It only works for message and key input of less than or equal to 128 bits. 

## Author

Tushar Sharma `tushar.sharma1729@gmail.com`

## License

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
