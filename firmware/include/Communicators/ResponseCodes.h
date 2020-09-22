#ifndef ResponseCodes_h
#define ResponseCodes_h

class ResponseCodes
{
    public:
    static const int GOOD_REQUEST_NO_REPLY = 0;
    static const int FORWARD_MESSAGE = 1;
    static const int BAD_REQUEST = 2;
    static const int GOOD_REQUEST_SEND_REPLY = 3;
    static const int UNKNOWN_REQUEST = 4;
};

#endif