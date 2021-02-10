## Main.hpp
### Classes
- `Main`   
    Initialize the system.  
    - Public static methods:
        - `void init(void);` Initialize.
## User.hpp
Wrapped in namespace `Bookstore`.
### Classes
- `User`
  - Inner classes:
    - `enum Permission`
      - `SUPERUSER = 7`
      - `ADMIN = 3`
      - `USER = 1`
      - `UNAUTHORIZED = 0`
  - Public fields:
    - `const Permission perm`
    - `std::string userName`
    - `std::string userID`
  - Private fields: 
    - `string passwd`
  - Constructors:
    - `User(std::string userID, std::string userName, Permission perm)`
  - Public methods:
    - `bool checkPasswd(const String& aPasswd)`
  - Private methods:


## UserStatus.hpp
Wrapped in namespace `Bookstore`.
### Headers Included
- `User.hpp`

### Classes
- `UserStatus`
    A singleton Class that has ONLY one instance in the program.  
    The class stores all status about users in the program.
    - Public fields:
      - `std::vector<User> userList`
      - `User currentUser`
    - Private fields:
      - `GlobalStatus *instance = nullptr`
    - Private constructor:
      - `GlobalStatus()`
    - Public methods:
      - `GlobalStatus& getInstance()` Returns the only instance. Lazy evaluation.
      - `void su()`

## IOBasic.hpp
Wrapped in namespace `Bookstore`.
### Classes
- `IOBasic`
  - Public fields:
    - 
  - Private fields:
    - istream in
      - 
    - ostream out
  - Constructors:
    - 
  - Public methods:
    - 
  - Private methods:
  - Public static methods:
    - 

## template
Wrapped in namespace `Bookstore`.
### Classes
- ``
  - Public fields:
    - 
  - Private fields: 
    - 
  - Constructors:
    - 
  - Public methods:
    - 
  - Private methods:
