# Interfacing in C

## Convention (Simulating OOP in C)

- Functions starting with \<type\> (Capital case) are constructors  
- All other functions are stored as function pointers inside the \<type\> struct  
  - Internal API Functions are prefixed with _, others are public  
  - Functions accepting self parameter are stateful methods  
  - Functions not accepting self parameter are stateless utils (and static associated functions only)  
  - In all methods accepting self param, if self param is null they do nothing just return
  - the \<type\> struct can contain an err struct for State Machine error information. The names in enums must be prefixed with type name  
  - Function with name 'free' is the destructor. The destructor must free all resources associated with the type. No method (except static ones) can return an independent freeable resource with a free function pointer, the methods must put resources in the type to be freed by the main destructor.  

## Normal functions

- All function must have standardized comments in header files.  
- To return multiple types, use a \<func\>_ret struct  
- To return a freeable resource, suffix the return type with _free, and provide a function pointer to the freeing function in the type.  
- To return possible error information:  
  - Use a sentinel (outside the range of normal values)  
  - use a \<func\>_err struct, and wrap both ret and err structs in a new struct named \<func\>_result. This also contains a field of Result enum indicating status. The err struct in Result enum based type must contain atleast 1 err code (as enum)  
