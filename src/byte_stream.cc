#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), error_(false), closed_(false), byte_stream_(), available_capacity_(capacity_), bytes_pushed_(0), bytes_buffered_(0), bytes_popped_(0){}

bool Writer::is_closed() const
{
  return closed_;;
}

void Writer::push( string data )
{
  if(is_closed())
    return;
  if(data.length() <= available_capacity()){//正常情况
    byte_stream_ += data;
    available_capacity_ -= data.length();
    bytes_pushed_ += data.length();
    bytes_buffered_ += data.length();
  }else{//data的长度大于 available_capacity_
    byte_stream_ += data.substr(0,available_capacity());
    bytes_pushed_ += available_capacity();
    bytes_buffered_ += available_capacity();
    available_capacity_ = 0;
  }
  return;
}

void Writer::close()
{
  closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  return available_capacity_;
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_;
}

bool Reader::is_finished() const
{
  if(byte_stream_.empty() && writer().is_closed())
    return true;
  return false;
}

uint64_t Reader::bytes_popped() const
{
  return bytes_popped_;
}

string_view Reader::peek() const
{
  return byte_stream_;
}

void Reader::pop( uint64_t len )
{
  if(is_finished())
    return;
  if( len < bytes_buffered_ ){//正常情况
    bytes_buffered_ -= len;
    available_capacity_ += len;
    bytes_popped_ += len;
    byte_stream_ = byte_stream_.substr(len);
  }else{//len比缓冲字节大
    available_capacity_ = capacity_;
    bytes_popped_ += bytes_buffered_;
    bytes_buffered_ = 0;
    byte_stream_.clear();
  }
}

uint64_t Reader::bytes_buffered() const
{
  return bytes_buffered_;
}
