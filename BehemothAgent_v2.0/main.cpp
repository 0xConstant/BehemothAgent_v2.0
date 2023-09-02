#include <Windows.h>
#include <iostream>
#include "file_enc.h"
#include "rsa_enc.h"


int main() {
    std::string base64PublicKey = "MIIBojANBgkqhkiG9w0BAQEFAAOCAY8AMIIBigKCAYEAsPhD11m4egevBM+OWL+jzUL+lgD3zDNv7kfAYCj1E1HXPDSc3PDnkl0fkfdk9VnIUjvkRdVg6fIj+Yn3q77V8YkutbJyEvgJtYR6GwHmvdXo+QSPNhDfhsat+GhJyTqP0Mmehpr4mln7vh60ShwJT8gjc7fk1ef08cr0GKR/oelffAx2jsu8JFg900XaZiYoZUXg877TNO9iTB1oQFRP0ugOs05BYuvhEsH6XnG+8l4VEIZP7Fr2w/TYWuu5fXFQfG6sv8nRtrl6te3J3NDZ7Zueq05wUfdYTxqOVh246CV/k/9IWGGcx3vlu+c9yxSAil7vr56Ubja9aE4IqG09cJh6O/FujYAIUQS4eg8EwyobcN+128qzXdkCdQt3uGXCm4R79NThtOGj2gU5vOnvZrlbSQsVOwDRPj03nvu6txvCvQb5CrWZMz++egMi3LmHHmLm1QgVBRPcDYG+VVg6Epf4mTjprvA5jan7LQOrismRvKou9aNZFl+5t2AVQlqzAgMBAAE=";

    std::string data = "Hello, World!";
    std::string encryptedData = RSAEncrypt(base64PublicKey, data);
    std::cout << "Encrypted Data: " << encryptedData << std::endl;

    return 0;
}
