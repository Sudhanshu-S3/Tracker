
> Starting with User Class we have two important things to decide
 1. What are the data the user hold like the Attributes
 2. What are the actions it can perform this is the methods we have to decide

> We have to store the User attributes Private to ensure security which is a known principle known as **Encapsulation**.

```cpp

#include <bits/stdc++.h>
using namespace std;

class User{
    private:

        userID
        userName
        passwordHash
        email
        profileInfo
        followers
        following
    
    public:
        void followerUser(userId){};
        void unfollowUser(userId){};
        createPost();
        getProfileDetails();
        updateProfile();
        
}

```

> The working code is in the files folder.