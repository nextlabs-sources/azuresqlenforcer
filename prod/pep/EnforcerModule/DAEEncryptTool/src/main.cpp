#include <string>
#include <iostream>

#include "CLI/CLI.hpp"

#include "commfun.h"

int main(int argc, char** argv)
{
	CLI::App app{ "A tool for encryption and decryption." };

	app.require_subcommand();

	{	// Subcommand for encrypting plaintext
		auto encrypt_command = app.add_subcommand("enc", "Encrypt the plaintext.");
		std::string plaintext;
		encrypt_command->add_option("-p,--plaintext", plaintext, "The plaintext to be encrypted.")->required();
		encrypt_command->callback([&]() {
			std::cout << "Ciphertext:\n" << CommonFun::EncryptString(plaintext) << std::endl;
		});
	}

	{	// Subcommand for decrypting ciphertext
		auto decrypt_command = app.add_subcommand("dec", "Decrypt the ciphertext.");
		std::string ciphertext;
		decrypt_command->add_option("-c,--ciphertext", ciphertext, "The ciphertext to be decrypted.")->required();
		decrypt_command->callback([&]() {
			std::cout << "Plaintext:\n" << CommonFun::DecryptString(ciphertext) << std::endl;
		});
	}

	CLI11_PARSE(app, argc, argv);

	return 0;
}