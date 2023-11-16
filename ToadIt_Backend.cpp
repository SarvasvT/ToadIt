#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class CommentNode {
public:
    string content;
    string author;
    string date;
    CommentNode* next;

    CommentNode(string content, string author, string date) {
        this->content = content;
        this->author = author;
        this->date = date;
        this->next = nullptr;
    }
};

class BlogPost {
private:
    string title;
    string content;
    string author;
    string date;
    vector<string> tags;
    vector<string> categories;
    CommentNode* comments; // Linked list of comments

public:
    BlogPost(string title, string content, string author, string date) {
        this->title = title;
        this->content = content;
        this->author = author;
        this->date = date;
        this->comments = nullptr;
    }

    string getTitle() const{
        return title;
    }
    string getContent() const{
        return content;
    }
    string getAuthor() const{
        return author;
    }
    string getDate() const{
        return date;
    }
    vector<string> getTags() const{
        return tags;
    }
    vector<string> getCategories() const{
        return categories;
    }

    CommentNode* getComments() const{
        return comments;
    }

    void addCategory(string category) {
        categories.push_back(category);
    }
    void deleteCategory(int index) {
        categories.erase(categories.begin() + index);
    }
    void editCategory(int index, string category) {
        categories[index] = category;
    }

    void addTag(string tag) {
        tags.push_back(tag);
    }
    void deleteTag(int index) {
        tags.erase(tags.begin() + index);
    }
    void editTag(int index, string tag) {
        tags[index] = tag;
    }

    void addComment(string content, string author, string date) {
        CommentNode* newComment = new CommentNode(content, author, date);
        if (!comments) {
            comments = newComment;
        } else {
            CommentNode* current = comments;
            while (current->next) {
                current = current->next;
            }
            current->next = newComment;
        }
    }

    void deleteComment(int index) {
        if (!comments || index < 0) {
            return;
        }
        if (index == 0) {
            CommentNode* temp = comments;
            comments = comments->next;
            delete temp;
        } else {
            CommentNode* current = comments;
            for (int i = 0; i < index - 1; i++) {
                if (!current) {
                    return; // Index out of bounds
                }
                current = current->next;
            }
            if (current && current->next) {
                CommentNode* temp = current->next;
                current->next = current->next->next;
                delete temp;
            }
        }
    }
    void editContent(string content) {
            this->content = content;
        }
};

class User {
public:
    string username;
    string password;
    vector<BlogPost> blogPosts;

public:
    User(string username, string password) {
        this->username = username;
        this->password = password;
    }

    void createBlogPost(string title, string content, string date) {
        BlogPost blogPost(title, content, username, date);
        blogPosts.push_back(blogPost);
    }

    void editBlogPost(int index, string content) {
        blogPosts[index].editContent(content);
    }

    void deleteBlogPost(int index) {
        blogPosts.erase(blogPosts.begin() + index);
    }

    BlogPost getBlogPost(int index) {
        return blogPosts[index];
    }
};

void readBlogsFromFile(vector<BlogPost>& blogPosts) {
    ifstream file("blogs.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line == "=== Blog Post ===") {
                string title, content, author, date;
                vector<string> tags, categories;
                getline(file, title);
                getline(file, content);
                getline(file, author);
                getline(file, date);
                
                string tag;
                while (file >> tag) {
                    if (tag == "Categories:") {
                        break;
                    }
                    tags.push_back(tag);
                }

                string category;
                while (file >> category) {
                    if (category == "Comments:") {
                        break;
                    }
                    categories.push_back(category);
                }

                blogPosts.emplace_back(title, content, author, date);
                BlogPost& currentPost = blogPosts.back();
                for (const auto &tag : tags) {
                    currentPost.addTag(tag);
                }
                for (const auto &category : categories) {
                    currentPost.addCategory(category);
                }

                string commentContent, commentAuthor, commentDate;
                while (getline(file, commentContent)) {
                    getline(file, commentAuthor);
                    getline(file, commentDate);
                    currentPost.addComment(commentContent, commentAuthor, commentDate);
                }
            }
        }
        file.close();
    }
}

void saveBlogsToFile(const vector<BlogPost>& blogPosts) {
    ofstream file("blogs.txt");
    if (file.is_open()) {
        for (const auto& post : blogPosts) {
            file << "=== Blog Post ===" << endl;
            file << "Title:" <<post.getTitle() << "\n";
            file << "Content:" <<post.getContent() << "\n";
            file << "Author:" <<post.getAuthor() << "\n";
            file << "Date:" <<post.getDate() << "\n";

            file << "Tags:";
            for (const auto& tag : post.getTags()) {
                file << " " << tag;
            }
            file << endl;

            file << "Categories:";
            for (const auto& category : post.getCategories()) {
                file << " " << category;
            }
            file << endl;

            CommentNode* comments = post.getComments();
            while (comments) {
                file << "Comment:";
                file << comments->content << endl;
                file << comments->author << endl;
                file << comments->date << endl;
                comments = comments->next;
            }
        }
        file.close();
    }
}


void showBlog(vector<BlogPost> blogPosts) {
    if (blogPosts.empty()) {
            cout << "No posts available.\n";
            return;
        }
        
    for (int i = 0; i < blogPosts.size(); i++) {
        cout << blogPosts[i].getTitle() << "\n";
        cout << blogPosts[i].getContent() << "\n";
        cout << blogPosts[i].getAuthor() << "\n";
        cout << blogPosts[i].getDate() << "\n";
        cout << "Tags: ";
        vector<string> tags = blogPosts[i].getTags();
        for (int j = 0; j < tags.size(); j++) {
            cout << tags[j] << " ";
        }
        cout << endl;
        cout << "Categories: ";
        vector<string> categories = blogPosts[i].getCategories();
        for (int j = 0; j < categories.size(); j++) {
            cout << categories[j] << " ";
        }
        cout << endl;
        cout << "Comments:" << "\n";
        CommentNode* comments = blogPosts[i].getComments();
        CommentNode* current = comments;
        while (current) {
            cout << "  " << current->content << " - " << current->author << " (" << current->date << ")" << "\n";
            current = current->next;
        }
        cout << "\n";
    }
}

void displayMenu() {
    cout << "1. Create a new blog post" << endl;
    cout << "2. Edit an existing blog post" << endl;
    cout << "3. Delete an existing blog post" << endl;
    cout << "4. Add a comment to a blog post" << endl;
    cout << "5. Delete a comment from a blog post" << endl;
    cout << "6. Show all Blogs" << endl;
    cout << "7. Exit" << endl;
}

int main() {
    vector<User> users;
    vector<BlogPost> blogPosts;
    readBlogsFromFile(blogPosts);

    // Create a new user
    User user("Jayant Goel", "password");
    users.push_back(user);

    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string title, content, date;
                cout << "Enter the title of the blog post: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter the content of the blog post: ";
                getline(cin, content);
                cout << "Enter the date of the blog post (YYYY-MM-DD): ";
                cin >> date;
                user.createBlogPost(title, content, date);
                blogPosts.push_back(user.getBlogPost(blogPosts.size()));
                cout << "Blog post created successfully!" << endl;
                break;
            }
            case 2: {
                int index;
                cout << "Enter the index of the blog post to edit: ";
                cin >> index;
                string content;
                cout << "Enter the new content of the blog post: ";
                cin.ignore();
                getline(cin, content);
                user.editBlogPost(index, content);
                blogPosts[index] = user.getBlogPost(index);
                cout << "Blog post edited successfully!" << endl;
                break;
            }
            case 3: {
                int index;
                cout << "Enter the index of the blog post to delete: ";
                cin >> index;
                user.deleteBlogPost(index);
                               blogPosts.erase(blogPosts.begin() + index);
                cout << "Blog post deleted successfully!" << endl;
                break;
            }
            case 4: {
                int index;
                cout << "Enter the index of the blog post to add a comment to: ";
                cin >> index;
                string content, author, date;
                cout << "Enter the content of the comment: ";
                cin.ignore();
                getline(cin, content);
                cout << "Enter the author of the comment: ";
                getline(cin, author);
                cout << "Enter the date of the comment (YYYY-MM-DD): ";
                cin >> date;
                blogPosts[index].addComment(content, author, date);
                cout << "Comment added successfully!" << endl;
                break;
            }
            case 5: {
                int postIndex, commentIndex;
                cout << "Enter the index of the blog post to delete a comment from: ";
                cin >> postIndex;
                cout << "Enter the index of the comment to delete: ";
                cin >> commentIndex;
                blogPosts[postIndex].deleteComment(commentIndex);
                cout << "Comment deleted successfully!" << endl;
                break;
            }
            case 6: {
                showBlog(blogPosts);
                break;
            }
            case 7: {
                saveBlogsToFile(blogPosts);
                cout << "Exiting program..." << endl;
                break;
            }
            default: {
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
    } while (choice != 7);

    // Clean up dynamically allocated memory
    for (User& user : users) {
        for (BlogPost& post : user.blogPosts) {
            CommentNode* current = post.getComments();
            while (current) {
                CommentNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    return 0;
}