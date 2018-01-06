#include <cstddef>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>

#include "schifra_galois_field.hpp"
#include "schifra_galois_field_polynomial.hpp"
#include "schifra_sequential_root_generator_polynomial_creator.hpp"
#include "schifra_reed_solomon_encoder.hpp"
#include "schifra_reed_solomon_decoder.hpp"
#include "schifra_reed_solomon_block.hpp"
#include "schifra_error_processes.hpp"

#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

/* Finite Field Parameters */
const std::size_t field_descriptor                =   8;
const std::size_t generator_polynomial_index      = 120;
const std::size_t generator_polynomial_root_count =  32;

/* Reed Solomon Code Parameters */
const std::size_t code_length = 255;
const std::size_t fec_length  =  32;
const std::size_t data_length = code_length - fec_length;

int trywholefile(std::ifstream &myfile) {
     while(!myfile.eof()) {
          std::string message;
          char c;
          while(myfile.get(c) && (message.length() < code_length)) {
               message.push_back(c);
          }
          /* Instantiate Finite Field and Generator Polynomials */
          const schifra::galois::field field(field_descriptor,
                                            schifra::galois::primitive_polynomial_size06,
                                            schifra::galois::primitive_polynomial06);

          schifra::galois::field_polynomial generator_polynomial(field);

          if (
               !schifra::make_sequential_root_generator_polynomial(field,
                                                                   generator_polynomial_index,
                                                                   generator_polynomial_root_count,
                                                                   generator_polynomial)
             )
          {
             std::cout << "Error - Failed to create sequential root generator!" << std::endl;
             return 0;
          }

          /* Instantiate Encoder and Decoder (Codec) */
          typedef schifra::reed_solomon::encoder<code_length,fec_length,data_length> encoder_t;
          typedef schifra::reed_solomon::decoder<code_length,fec_length,data_length> decoder_t;

          const encoder_t encoder(field, generator_polynomial);
          const decoder_t decoder(field, generator_polynomial_index);

          /* Pad message with nulls up until the code-word length */
          message.resize(code_length,0x00);

          //std::cout << "Original Message:  [" << message << "]" << std::endl;

          /* Instantiate RS Block For Codec */
          schifra::reed_solomon::block<code_length,fec_length> block;

          /* Transform message into Reed-Solomon encoded codeword */
          if (!encoder.encode(message, block))
          {
             std::cout << "Error - Critical encoding failure! "
                       << "Msg: " << block.error_as_string()  << std::endl;
             return 0;
          }

          /* Add errors at every 3rd location starting at position zero */
          schifra::corrupt_message_all_errors00(block, 0, 3);

          //std::cout << "Corrupted Codeword: [" << block << "]" << std::endl;

          if (!decoder.decode(block))
          {
             std::cout << "Error - Critical decoding failure! "
                       << "Msg: " << block.error_as_string()  << std::endl;
             return 0;
          }
          else if (!schifra::is_block_equivelent(block, message))
          {
             std::cout << "Error - Error correction failed!" << std::endl;
             return 0;
          }

          block.data_to_string(message);

          //std::cout << "Corrected Message: [" << message << "]" << std::endl;
     }
     return 1;
}


int main(const int argc, const char **argv) {
     if(argc != 2) {
          LOG(FATAL) << "Must enter a filename!";
          return 1;
     }

     std::ifstream myfile(argv[1]);
     if(!myfile.is_open()) {
          LOG(FATAL) << "Could not open specified file: <" << argv[1] << ">";
          return 2;
     }

     int good = trywholefile(myfile);

     if(good)
          std::cout << "Success" << std::endl;
     else
          std::cout << "Failure" << std::endl;

     myfile.close();
     return 0;
}
