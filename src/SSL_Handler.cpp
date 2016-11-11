//Initialize the SSl environment
BIO* init(SSL_CTX *& ctx){
    //Initialize SSL
    ERR_load_crypto_strings();
    ERR_load_SSL_strings();
    SSL_library_init();
    SSL_load_error_strings();
    ERR_load_BIO_strings();    
    OpenSSL_add_all_algorithms();

    //Setting up SSL pointers
    ctx = SSL_CTX_new(SSLv23_client_method());
    SSL * ssl;

    //Loading the certificates
    if(!SSL_CTX_load_verify_locations(ctx, /*"UbuntuOne-Go_Daddy_CA.pem",*/ NULL, "/etc/ssl/certs/")){
        cerr << "Loading the certs file failed" << endl;
        exit(1);
    }
    

    //Setup the BIO
    BIO * bio = BIO_new_ssl_connect(ctx);
    BIO_get_ssl(bio, & ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
 
    //Check ssl pointer
    if(!ssl){
        cerr << "Cannot find ssl pointer" << endl;
        exit(1);
    }

    //Attempts a connection
    BIO_set_conn_hostname(bio, ("imgur.com:" + PORT + "asdfas").c_str());

    //Check connection
    if(BIO_do_connect(bio) <= 0){
        cerr << "Connection Failed" << endl;
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        exit(1);
    }

    //Check certificate verification
    if(SSL_get_verify_result(ssl) != X509_V_OK){
        cout << "Connection not secured. Do you want to continue[Y/n]?" << endl;
        char ans;
        cin >> ans;
    
        if(ans == 'n'){
            exit(1);
        }
    }
    
    //Handshake
    if(BIO_do_handshake(bio) <= 0){
        cerr << "Cannot establish SSL connection" << endl;
        exit(1);
    }
    return bio;
}

//Clean SSL context
void SSL_Handler::clean_SSL_Environment(){
    BIO_free_all(bio);
    SSL_CTX_free(ctx);
}