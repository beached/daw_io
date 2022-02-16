# daw_io
modern type erased io library

## output_stream
```c++
struct daw::io::output_stream {
  virtual void write( std::span<std::byte const> data ) = 0;
  virtual void close( );
  virtual void flush( );
  [[nodiscard]] virtual bool can_write( ) const;
  
  void write( std::span<std::byte const> data, std::size_t offset, std::size_t length );
  
  template<typename...Ts> 
  void print( fmt::format_string fmt, Ts &&...args );
  
  template<typename...Ts> 
  void print( std::string_view fmt, Ts &&...args );
};
```
    
