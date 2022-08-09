
#include <sstream>

//------------------------

#include "m4th.h"
#include "algorithm.h"

class RSA {
	
	// public (e)
	// private (d)
	
	private:
		
		int p, q, e;    
		Long b, n, d;
		
		Long gen_d(Long max=999999999);  				// gen d having e, and b = (p-1)*(q-1)
		int gen_e(int max=99999);						// gen e having b = (p-1)*(q-1)
	
	public:
		
		RSA(bool=1);
		
		std::string encrypt(std::string, bool=1);
							
		// int[] = (a-b-c-d), int = array size
		std::string decrypt(int[], int, bool=1); // deprecated
		std::string decrypt(std::string, bool=1);
		
		void show_pars();
		
		std::string get_pbc_key();
		std::string get_prv_key();
		
		void set_d(Long);
		void set_n(Long);
		void set_e(int);
		
		// encrypt with public key and decrypt with private key => encrypt(string, int, bool=1), decrypt(int[], int, bool=1);
		// encrypt with private key and decrypt with public key => encrypt(string, int, bool=0), decrypt(int[], int, bool=0);
};

RSA::RSA(bool gen){
	
	if (gen == 0)
	{
		this->p = -1;
		this->q = -1;
		this->b = -1;
		this->n = -1;
		this->e = -1;
		this->d = -1;
		return;
	}
	
	this->p = MATH::generate_prime(5,100);
	
	int temp_q = 0;
	
	do {
		
		temp_q = MATH::generate_prime(5,100);
		
		if (temp_q == p) continue; else break;
		
	} while (true);
	
	this->q = temp_q;
	
	this->b = (p-1) * (q-1);
	this->n = p * q;
	
	this->e = gen_e();
	this->d = gen_d();
}

void RSA::set_d(Long d){
	if (d > 0)
		this->d = d;
}

void RSA::set_n(Long n){
	if (n > 0)
		this->n = n;
}

void RSA::set_e(int e){
	if (e > 0)
		this->e = e;
}

std::string RSA::encrypt(std::string plain, bool pbc_key){
		
	int array_msg_plain[plain.length()];
	
	for (int i=0; i<plain.length(); i++)
		array_msg_plain[i] = (int)plain[i];
	
	int enc[plain.length()];
	
	auto encryption_key = pbc_key ? this->e : this->d;
	
	for (int i=0; i<plain.length(); i++)
		enc[i] = MATH::modularExp(array_msg_plain[i], encryption_key, this->n);
		
	std::string str_enc = "";
		
	for (int i=0; i<plain.length(); i++)
	{
		std::ostringstream temp;
    	temp << enc[i];
    	str_enc += temp.str();
    	if (i != plain.length() - 1) str_enc += "-";
	}

	return b64encode(str_enc);
}

// input int array required (example) => 8000-8001-8002
std::string RSA::decrypt(int crypted[], int size, bool prv_key){
	
	int decrypted_message[size];
	
	std::string decrypted = "";
	
	auto decryption_key = prv_key ? d : e;
	
	for (int i=0; i<size; i++)
	{
		decrypted_message[i] = MATH::modularExp(crypted[i], decryption_key, n);
		decrypted += (char)decrypted_message[i];
	}
	
	return decrypted;
}

// input string required (example) => "XJhu23xjnwx23=="
std::string RSA::decrypt(std::string crypted, bool prv_key){
	
	std::string decoded_str = b64decode(crypted);
	
	std::vector<std::string> str_array = split(decoded_str);
	
	int int_array[str_array.size()];
	
	std::string plain = "";
	
	auto decryption_key = prv_key ? d : e;
	
	for (int i=0; i<str_array.size(); i++)
	{
		int_array[i] = std::stoi(str_array[i]);
		plain += (char)MATH::modularExp(int_array[i], decryption_key, n);
	}
	
	return plain;
}

void RSA::show_pars(){
	
	std::cout << "\n\n";
	std::cout << " -- PARAMETRI PER RSA --\n\n --------------------------------------------------- \n\n";
	std::cout << "  p: "<<this->p<<std::endl;
	std::cout << "  q: "<<this->q<<std::endl; 
	std::cout << "  b: "<<this->b<<"    (p-1) * (q-1)\n"; 
	std::cout << "  n: "<<this->n<<"    (p)   * (q)  \n"; 
	std::cout << "  e: "<<this->e<<std::endl; 
	std::cout << "  d: "<<this->d<<std::endl;
	std::cout << "\n --------------------------------------------------- ";  
	std::cout << "\n\n";
}

Long RSA::gen_d(Long max) {
	
	for (int d=0; d<max; d++) 
		if ((d * e) % b == 1) 
			return d;
	return -1;
}

int RSA::gen_e(int max) {
	
	int e = 2;
	
	while (e < max) 
		if (MATH::MCD(e, b) == 1) 
			return e; 
		else 
			e++;
	
	return -1;
}

std::string RSA::get_pbc_key()
{
	std::ostringstream temp;
    temp << e;
    std::string es = temp.str();
	std::string b64 = b64encode(es);
	return b64;
}

std::string RSA::get_prv_key()
{	
	std::ostringstream temp;
    temp << d;
    std::string ds = temp.str();
	std::string b64 = b64encode(ds);
	return b64;
}



