#include "tcp_receiver.hh"

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  if(message.SYN){
    zero_point_ = message.seqno;
    isSYN_ = true;
  }
  if(isSYN_ && message.FIN)
    isFIN_ = true;
   if (message.SYN) {
    reassembler_.insert( (message.seqno + 1).unwrap( zero_point_, writer().bytes_pushed() ) - 1,
                         message.payload, message.FIN );
  }else{
    reassembler_.insert( message.seqno.unwrap( zero_point_, writer().bytes_pushed() ) - 1,
                         message.payload, message.FIN );
  }
  //uint64_t first_index = (message.seqno + message.SYN).unwrap(zero_point_,writer().bytes_pushed()-1);
  //reassembler_.insert(first_index,message.payload,message.FIN);
  if(message.RST)
    reader().set_error();
}

TCPReceiverMessage TCPReceiver::send() const
{
  TCPReceiverMessage message;
  //计算window_size
  uint16_t window_size = UINT16_MAX;
  if(writer().available_capacity() < window_size)
    window_size = writer().available_capacity();
  //计算seqno
  if(!isSYN_)//还没收到SYN
    message = {nullopt,window_size,reader().has_error()};
  else{
    Wrap32 ackno = Wrap32::wrap(writer().bytes_pushed() + 1, zero_point_);
    if(isFIN_ && reassembler_.bytes_pending() == 0)
      ackno = ackno + 1;
    message = {ackno,window_size,reader().has_error()};
  }
  return message;
}
