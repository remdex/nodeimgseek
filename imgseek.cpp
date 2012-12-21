/* This code is PUBLIC DOMAIN, and is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND. See the accompanying 
 * LICENSE file.
 */

#include <v8.h>
#include <node.h>
#include "bloom_filter.h"
#include "imgdb.h"

using namespace node;
using namespace v8;

class ImgSeek: ObjectWrap
{
private:
  int m_count;
  Persistent< Value > host;
public:

  static Persistent<FunctionTemplate> s_ct;
  static void Init(Handle<Object> target)
  {
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);

    s_ct = Persistent<FunctionTemplate>::New(t);
    s_ct->InstanceTemplate()->SetInternalFieldCount(1);
    s_ct->SetClassName(String::NewSymbol("ImgSeek"));
 
    NODE_SET_PROTOTYPE_METHOD(s_ct, "loadalldbs", Loadalldbs);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "createdb", CreateDb);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "savealldbs", SaveAllDbs);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "resetdb", ResetDb);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "removedb", RemoveDb);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "getimgcount", getImgCountSeek);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "addimage", AddImageSeek);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "isimageondb", isImageOnDBSeek);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "queryimgid", queryImgIDSeek);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "queryimgidfast", queryImgIDFastSeek);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "removeid", removeIDSeek);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "isvaliddb", isValidDBSeek);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "queryimgfile", queryImgFileSeek);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "queryimgfilefast", queryImgFileFastSeek);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "getdblist", getDBListSeek);

    target->Set(String::NewSymbol("ImgSeek"),
                s_ct->GetFunction());
  }

  ImgSeek() 
  {
        
  }

  ~ImgSeek()
  {
  
  }
  
  void saveDatabase()
  {
    //initDbase(1);     
    //initDbase(1);  
    //addImage(1,1,"/home/remdex/nodeimgseek/artisticaddiction.jpg");
        
    //loadalldbs("./isk-db");
    //addImage(1,3,"/home/remdex/nodeimgseek/artisticaddiction.jpg");
    //savealldbs("./isk-db");   
  }
  
  static Handle<Value> New(const Arguments& args)
  {
    HandleScope scope;
    ImgSeek* hw = new ImgSeek();
    hw->Wrap(args.This());
    return args.This();
  }
  
  static Handle<Value> Loadalldbs(const Arguments& args)
  {  
    v8::String::Utf8Value v8str(args[0]);
    return v8::Integer::New(loadalldbs(*v8str)); 
  }

  static Handle<Value> AddImageSeek(const Arguments& args)
  {
    int db_id = args[0]->ToInteger()->Value();
    long int pid = args[1]->ToInteger()->Value();
    v8::String::Utf8Value fname(args[2]);
        
    if (isValidDB(db_id)) {    
        return v8::Integer::New(addImage(db_id,pid,*fname)); 
    } else {
        return v8::Integer::New(0);
    }
  }

  static Handle<Value> getImgCountSeek(const Arguments& args)
  {
    int db_id = args[0]->ToInteger()->Value();
    return v8::Integer::New(getImgCount(db_id)); 
  }
  
  static Handle<Value> SaveAllDbs(const Arguments& args)
  {
    v8::String::Utf8Value v8str(args[0]);
    return v8::Integer::New(savealldbs(*v8str)); 
  }  
  
  static Handle<Value> CreateDb(const Arguments& args)
  {  
    int db_id = args[0]->ToInteger()->Value();
    if (!isValidDB(db_id))
    {
         initDbase(db_id);
         return v8::Boolean::New(true); // New datatabase created
    } 
    else 
    { 
         return v8::Boolean::New(false); // Database already exists
    }    
  }
  
  static Handle<Value> ResetDb(const Arguments& args)
  {  
    int db_id = args[0]->ToInteger()->Value();
    if (resetdb(db_id))
    {
         return v8::Boolean::New(true); // New datatabase created
    } 
    else 
    { 
         return v8::Boolean::New(false); // Database already exists
    }    
  }
  
  static Handle<Value> RemoveDb(const Arguments& args)
  {  
    return v8::Boolean::New(removedb(args[0]->ToInteger()->Value()));  
  }
  
  static Handle<Value> isImageOnDBSeek(const Arguments& args)
  {  
    int db_id = args[0]->ToInteger()->Value();
    long int pid = args[1]->ToInteger()->Value();    
    return v8::Boolean::New(isImageOnDB(db_id,pid));   
  }
  
  static Handle<Value> removeIDSeek(const Arguments& args)
  {  
    int db_id = args[0]->ToInteger()->Value();
    long int pid = args[1]->ToInteger()->Value(); 
       
    return v8::Integer::New(removeID(db_id,pid));   
  }
  
  static Handle<Value> isValidDBSeek(const Arguments& args)
  {    
    return v8::Boolean::New(isValidDB(args[0]->ToInteger()->Value()));   
  }
  
  static Handle<Value> getDBListSeek(const Arguments& args)
  {    
    HandleScope scope;
    
    std::vector<int> dblist = getDBList();    
    Local<Array> tuples = Array::New(dblist.size());
    
    vector<int>::iterator it;
    int counter = 0;
    for ( it=dblist.begin() ; it < dblist.end(); it++ )
    {
        tuples->Set(Integer::New(counter), Integer::New(*it));
        counter++;
    }
    
    return scope.Close(tuples);   
  }

  static Handle<Value> queryImgIDSeek(const Arguments& args)
  {  
    HandleScope scope;
  
    int db_id = args[0]->ToInteger()->Value();
    long int pid = args[1]->ToInteger()->Value();     
    int numres = args[2]->ToInteger()->Value();
    
    Local<Array> tuples = Array::New(numres);
        
    std::vector<double> result = queryImgID(db_id,pid,numres);
    int found = result.size()/2;
    
    for (int i = 0;i < found; i++) {
        Local<Object> row = Object::New();
        tuples->Set(Integer::New(found-i-1), row);
        row->Set(String::New("id"), Number::New(result[i*2]));        
        double rsc = (-100.0*result[i*2+1]/38.70);        
        if (rsc<0) rsc = 0;
        if (rsc>100) rsc = 100;                
        row->Set(String::New("cor"), Number::New(rsc));        
    }
    
    return scope.Close(tuples);
  }
  
  static Handle<Value> queryImgIDFastSeek(const Arguments& args)
  {  
    HandleScope scope;
  
    int db_id = args[0]->ToInteger()->Value();
    long int pid = args[1]->ToInteger()->Value();     
    int numres = args[2]->ToInteger()->Value();
    
    Local<Array> tuples = Array::New(numres);
        
    std::vector<double> result = queryImgIDFast(db_id,pid,numres);
    int found = result.size()/2;
    
    for (int i = 0;i < found; i++) {
        Local<Object> row = Object::New();
        tuples->Set(Integer::New(found-i-1), row);
        row->Set(String::New("id"), Number::New(result[i*2]));        
        double rsc = (-100.0*result[i*2+1]/38.70);        
        if (rsc<0) rsc = 0;
        if (rsc>100) rsc = 100;                
        row->Set(String::New("cor"), Number::New(rsc));        
    }
    
    return scope.Close(tuples);
  }
  
  static Handle<Value> queryImgFileSeek(const Arguments& args)
  {  
    HandleScope scope;
  
    v8::String::Utf8Value fname(args[0]);    
    int db_id = args[1]->ToInteger()->Value();
    int numres = args[2]->ToInteger()->Value();
    
    Local<Array> tuples = Array::New(numres);
        
    std::vector<double> result = queryImgFile(*fname, db_id, numres, args[3]->ToInteger()->Value());
    int found = result.size()/2;
    
    for (int i = 0;i < found; i++) {
        Local<Object> row = Object::New();
        tuples->Set(Integer::New(found-i-1), row);
        row->Set(String::New("id"), Number::New(result[i*2]));        
        double rsc = (-100.0*result[i*2+1]/38.70);        
        if (rsc<0) rsc = 0;
        if (rsc>100) rsc = 100;                
        row->Set(String::New("cor"), Number::New(rsc));        
    }
    
    return scope.Close(tuples);
  }
  
  static Handle<Value> queryImgFileFastSeek(const Arguments& args)
  {  
    HandleScope scope;
  
    v8::String::Utf8Value fname(args[0]);    
    int db_id = args[1]->ToInteger()->Value();
    int numres = args[2]->ToInteger()->Value();
    
    Local<Array> tuples = Array::New(numres);
        
    std::vector<double> result = queryImgFileFast(*fname, db_id, numres, args[3]->ToInteger()->Value());
    int found = result.size()/2;
    
    for (int i = 0;i < found; i++) {
        Local<Object> row = Object::New();
        tuples->Set(Integer::New(found-i-1), row);
        row->Set(String::New("id"), Number::New(result[i*2]));        
        double rsc = (-100.0*result[i*2+1]/38.70);        
        if (rsc<0) rsc = 0;
        if (rsc>100) rsc = 100;                
        row->Set(String::New("cor"), Number::New(rsc));        
    }
    
    return scope.Close(tuples);
  }
  
};

Persistent<FunctionTemplate> ImgSeek::s_ct;

extern "C" {
  static void init (Handle<Object> target)
  {
    ImgSeek::Init(target);
  }

  NODE_MODULE(imgseek, init);
}
