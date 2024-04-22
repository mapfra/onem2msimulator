// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com

//
// TODO generated message class
//
packet OneM2MPrimitive {
    int messageID;
    
    string source;
    string destination;
    string from;
    string to;
    
    bool isRequest;
    int operation;
    int resourceType;
    string resourceName;    
    string uri;
    string content;
    int status;
    
    
    //Network Layer
    string srcAddress;
    string destAddress;
    int protocol;
}
