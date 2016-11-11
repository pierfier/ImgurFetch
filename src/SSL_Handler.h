#ifndef
#define

class SSL_Handler{

	public:
		SSL_Handler();
		~SSL_Handler();
	public:
		SSL_CTX * ctx;
    	BIO * bio;
};

#endif