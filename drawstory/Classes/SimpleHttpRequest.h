//
//  SimpleHttpRequest.h
//  drawstory
//
//  Created by 张 靖宇 on 12-3-30.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_SimpleHttpRequest_h
#define drawstory_SimpleHttpRequest_h

#include "cocos2d.h"
#include "pthread.h"
#include "curl/curl.h"
#include "json/json.h"
#include <string>


class SimpleHttpRequestDelegate
{
public:
    virtual void onResponse(bool result,const Json::Value& response) = 0;
};


class SimpleHttpRequest : public cocos2d::CCObject
{
public:
    typedef enum {
        kHttpMethodGet,
        kHttpMethodPost,
        kHttpMethodPut,
        kHttpMethodDelete
    } HttpMethod;
    
public:
    static SimpleHttpRequest* simpleHttpRequestWithURL(const std::string& url,
                                                       HttpMethod method,
                                                       SimpleHttpRequestDelegate* delegate);
public:
    SimpleHttpRequest(const std::string& url,HttpMethod method,SimpleHttpRequestDelegate* delegate);
    virtual ~SimpleHttpRequest();
    
    // Operator
    void cancel();
    bool cancelled() const { return cancelled_; }
    
    // start 
    bool start();
    
    
    void setURL(const std::string& url) { url_ = url; }
    const std::string& url() const { return url_; }
    
    void setHttpMethod(HttpMethod method) { method_ = method; }
    HttpMethod httpMethod() const { return method_; }
    
    void setPostField(const std::string& postField) { postField_ = postField; }
    const std::string& postField() const { return postField_; }
    

protected:    
    static size_t writeData(char *data, size_t size, size_t nmemb, SimpleHttpRequest *userdata);
    std::string& buffer() { return buffer_; }
    
    
    static void* doRequest(void* param);
protected:
    pthread_t thread_; 
    pthread_mutex_t mutex_;
    
    // need lock mutex
    bool cancelled_;
    
    SimpleHttpRequestDelegate* delegate_;
    // URL
    std::string url_;
    //
    HttpMethod method_;
    //
    std::string postField_;
    // 
    std::string buffer_;

};


#endif