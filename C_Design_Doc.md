# Convention (Simulating OOP in C)

- Functions starting with List_ are constructors
- All other functions are stored as function pointers inside the list struct
  - Function with name free is the destructor  
  - Internal API Functions are prefixed with _  
  - Functions accepting self parameter are stateful methods  
  - Functions not accepting self parameter are stateless utils (and static associated functions only)  
