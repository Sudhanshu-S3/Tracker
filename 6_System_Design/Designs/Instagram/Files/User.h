#pragma once //Ensure the file is included only once in a single compilation

#include <string>
#include <vector>
#include <iostream>

class Post;

class User {

    private:
        int userId;
        std::string username;
        std::string passwordHash;
        std::string email;
        std::string profileInfo;
        std::vector<int> followers;
        std::vector<int> following;
        std::vector<Post> posts;
        std::vector<int> likedPosts;
        
    public:
        //Constructors to initialize User Objects

        User(int id, const std::string& username, const std::string& passwordHash, const std::string& email);

        void followUser(int userId);
        void unfollowUser(int userIdToUnfollow);
        
        //Getters to access private data safely
        int getId() const;
        std::string getUsername() const;
        void viewProfile() const;

};