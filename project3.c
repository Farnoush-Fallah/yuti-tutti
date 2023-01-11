#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
typedef struct profile PROFILE;
typedef struct post POST;
struct post
{
    char *user;
    char *text;
    int postID;
    int likes;
    int *likedBy;
    POST *next;
};
struct profile
{
    char *userName;
    char *password;
    int profileID;
    POST *postHead;
    PROFILE *next;
};
void checkMemory(void *memory);
int whichOrder(char *order, char *str2, char *str3);
void whichAction(int status, char *str2, char *str3, PROFILE *profileHead, int *activeUser, int *AlreadyInFunction);
void signup(PROFILE *profileHead, char *userName, char *password, int AlreadyInFunction);
void login(PROFILE *profileHead, int *activeUser, char *userName, char *password, int *AlreadyInFunction);
void getTextTillSpace(char **destination);
int getOrder(char **order, char **str2, char **str3);
void getLine(char **destination);
void post(PROFILE *profileHead, int activeUser);
void like(PROFILE *profileHead, int activeUser, char *username, char *postID);
void delete(PROFILE *profileHead, int activeUser, char *postID);
void logout(int *activeUSer, int *AlreadyInFunction);
void info(PROFILE *profileHead, int activeUser, int alreadyInFunction);
void find_user(PROFILE *profileHead, char *userName, int activeUser);
void writeIntoFile(PROFILE *profileHead, FILE *accounts, FILE *posts);

int main(void)
{
    int status, size2, size3;
    int activeUser = 0; /*the user who is actively logged in*/
    int AlreadyInFunction = 0;
    char *order;
    char *str2;
    char *str3;
    FILE *accounts;
    FILE *posts;
    /*creating the head of the profiles*/
    /*the head doesn't save a real profile info*/
    PROFILE *profileHead = (PROFILE *)malloc(sizeof(PROFILE));
    checkMemory((void *)profileHead);
    profileHead->profileID = 0;
    profileHead->next = NULL;
    order = (char *)malloc(sizeof(char));
    checkMemory((void *)order);
    str2 = (char *)malloc(sizeof(char));
    checkMemory((void *)str2);
    str3 = (char *)malloc(sizeof(char));
    checkMemory((void *)str3);

    /*start the program*/
    while (TRUE)
    {
        int endOrNot = 1;
        printf("\nEnter your order, 0 if you want to end the program!\n");
        // endOrNot = getOrder(&order, &str2, &str3);
        endOrNot = getOrder(&order, &str2, &str3);
        if (endOrNot == 0)
        {
            printf("\nThank you for using this program. See you soon!\n");
            free(order);
            free(profileHead);
            return 0;
        }
        status = whichOrder(order, str2, str3);
        if (status == 0)
        {
            continue;
        }
        whichAction(status, str2, str3, profileHead, &activeUser, &AlreadyInFunction);
        fflush(stdin);
        writeIntoFile(profileHead, accounts, posts);
        free(order);
        writeIntoFile(profileHead, accounts, posts);
        order = (char *)malloc(sizeof(char));
        checkMemory((void *)order);
        str2 = (char *)malloc(sizeof(char));
        checkMemory((void *)str2);
        str3 = (char *)malloc(sizeof(char));
        checkMemory((void *)str3);
    }
}

/***************************************************
checks if memory is allocated successfully
****************************************************/
void checkMemory(void *memory)
{
    if (memory == NULL)
    {
        printf("Memory allocation failed!\n");
    }
}

/*********************************************************************************
reads text from stdin buffer until reaches ' ' or '\n' and stores into destination
**********************************************************************************/
void getTextTillSpace(char **destination)
{
    char tempChar = '\0';
    int lettersFound = 0;
    while (TRUE)
    {
        tempChar = getchar();
        lettersFound++;
        (*destination) = (char *)realloc(*destination, lettersFound * sizeof(char));
        if (tempChar != '\n' && tempChar != ' ')
        {
            (*destination)[lettersFound - 1] = tempChar;
        }
        else
        {
            (*destination)[lettersFound - 1] = '\0';
            break;
        }
    }
}

/***************************************************************
gets three words from buffer and saves them three dynamic arrays
returns 0 if order is to end the program
returns 1 otherwise
****************************************************************/
int getOrder(char **order, char **str2, char **str3)
{
    getTextTillSpace(order);
    if (strcmp(*order, "0") == 0)
    {
        return 0;
    }
    if ((strcmp(*order, "post") == 0) || (strcmp(*order, "logout") == 0) || (strcmp(*order, "info") == 0))
    {
        return 1;
    }
    getTextTillSpace(str2);
    if ((strcmp(*order, "delete") == 0) || strcmp(*order, "find_user") == 0) /*we don't need str3 :) */
    {
        *str3 = NULL;
        return 1;
    }
    getTextTillSpace(str3);
    return 1;
}

/*****************************************************************************************
returns the ordered number, used in another function with a switch to call suitable funcs
******************************************************************************************/
int whichOrder(char *order, char *str2, char *str3)
{
    int status;
    int flagValidEntry = 0;
    /*comparing the order to all the default orders*/
    /*if it's not equal to none, recalls the function*/
    if (strcmp(order, "signup") == 0)
    {
        flagValidEntry = 1;
        status = 1;
    }
    if (strcmp(order, "login") == 0)
    {
        flagValidEntry = 1;
        status = 2;
    }
    if (strcmp(order, "post") == 0)
    {
        flagValidEntry = 1;
        status = 3;
    }
    if (strcmp(order, "like") == 0)
    {
        flagValidEntry = 1;
        status = 4;
    }
    if (strcmp(order, "delete") == 0)
    {
        flagValidEntry = 1;
        status = 5;
    }
    if (strcmp(order, "logout") == 0)
    {
        flagValidEntry = 1;
        status = 6;
    }
    if (strcmp(order, "info") == 0)
    {
        flagValidEntry = 1;
        status = 7;
    }
    if (strcmp(order, "find_user") == 0)
    {
        flagValidEntry = 1;
        status = 8;
    }
    if (flagValidEntry == 0)
    {
        printf("\nInvalid order!\n");
        fflush(stdin);
        status = 0;
        return status;
    }
    return status;
}

/*****************************************************
calls specific functions according to user's order
******************************************************/
void whichAction(int status, char *str2, char *str3, PROFILE *profileHead, int *activeUser, int *AlreadyInFunction)
{
    switch (status)
    {
    case 1:
        /*call signup func*/
        signup(profileHead, str2, str3, *AlreadyInFunction);
        break;
    case 2:
        /*call login func*/
        login(profileHead, activeUser, str2, str3, AlreadyInFunction);
        break;
    case 3:
        /*call post func*/
        post(profileHead, *activeUser);
        break;
    case 4:
        /*call like func*/
        like(profileHead, *activeUser, str2, str3);
        break;
    case 5:
        /*call delete func*/
        delete (profileHead, *activeUser, str2);
        break;
    case 6:
        /*call its func*/
        logout(activeUser, AlreadyInFunction);
        break;
    case 7:
        /*call info func*/
        info(profileHead, *activeUser, *AlreadyInFunction);
        break;
    case 8:
        /*call find_user func*/
        find_user(profileHead, str2, *activeUser);
        break;
    }
}

/****************************************************************
signs the user up; if user name is already used, an error occurs
*****************************************************************/
void signup(PROFILE *profileHead, char *userName, char *password, int AlreadyInFunction)
{
    /*checking if we're already logged into another account*/
    if (AlreadyInFunction != 0)
    {
        printf("\nYou need to log out first!\n");
        return;
    }
    PROFILE *currNode = profileHead;
    PROFILE *prevNode = profileHead;
    while (currNode != NULL)
    {
        if (strcmp(userName, currNode->userName) == 0)
        {
            free(userName);
            free(password);
            printf("Username already taken!\n");
            return;
        }
        prevNode = currNode;
        currNode = currNode->next;
    }
    PROFILE *lastProfile = prevNode;
    PROFILE *newUser = (PROFILE *)malloc(sizeof(PROFILE));
    checkMemory((void *)newUser);
    lastProfile->next = newUser;
    newUser->userName = userName;
    newUser->password = password;
    newUser->profileID = lastProfile->profileID + 1;
    newUser->next = NULL;
    newUser->postHead = (POST *)malloc(sizeof(POST));
    newUser->postHead->likes = 0;
    printf("\nUser is singed up successfully!\n");
}

/********************************************************************************************
determines if username and password are correct; logs user in; makes changes in their profile
*********************************************************************************************/
void login(PROFILE *profileHead, int *activeUser, char *userName, char *password, int *AlreadyInFunction)
{
    /*user cannot be logged into two accounts at the same time*/
    if (*activeUser != 0)
    {
        printf("You're already in another acount! Logout first!\n");
        return;
    }
    int LoginSuccessful = 0;
    PROFILE *currNode = profileHead;
    PROFILE *prevNode = profileHead;
    while (currNode != NULL)
    {
        if (strcmp(userName, currNode->userName) == 0)
        {
            if (strcmp(password, currNode->password) == 0)
            {
                LoginSuccessful = 1;
                *activeUser = currNode->profileID;
                printf("\nlogin successful.");
                *AlreadyInFunction = 1;
                free(userName);
                free(password);
                return;
            }
            else
            {
                printf("\nPassword is incorrect! Try again.\n");
                free(userName);
                free(password);
                return;
            }
        }
        prevNode = currNode;
        currNode = currNode->next;
    }
    if (LoginSuccessful == 0)
    {
        printf("\nlogin failed! No account with such username! Try again.\n");
        free(userName);
        free(password);
        return;
    }
}

/**************************************
reads a line from text in buffer
***************************************/
void getLine(char **destination)
{
    char tempChar = '\0';
    int lettersFound = 0;
    while (TRUE)
    {
        tempChar = getchar();
        lettersFound++;
        (*destination) = (char *)realloc(*destination, lettersFound * sizeof(char));
        if (tempChar != '\n')
        {
            (*destination)[lettersFound - 1] = tempChar;
        }
        else
        {
            (*destination)[lettersFound - 1] = '\0';
            break;
        }
    }
    printf("%s", *destination);
}

/*****************************************************************************************
receives the text which the user wants to post; then, makes changes in the post linked list
******************************************************************************************/
void post(PROFILE *profileHead, int activeUser)
{
    if (activeUser == 0)
    {
        printf("You're not logged in!!\n");
        return;
    }
    PROFILE *selectedProfile;
    PROFILE *currNode = profileHead->next;
    PROFILE *prevNode = profileHead;
    while (currNode != NULL)
    {
        if (currNode->profileID == activeUser)
        {
            selectedProfile = currNode;
        }
        prevNode = currNode;
        currNode = currNode->next;
    }
    /*find the last post of the selected profile*/
    POST *currPost = selectedProfile->postHead;
    POST *prevPost = selectedProfile->postHead;
    POST *lastPost;
    while (currPost != NULL)
    {
        prevPost = currPost;
        lastPost = prevPost;
        currPost = currPost->next;
    }
    lastPost->next = (POST *)malloc(sizeof(POST));
    lastPost->next->postID = lastPost->postID + 1;
    char *temp = (char *)malloc(sizeof(char));
    getLine(&temp);
    lastPost->next->text = temp;
    lastPost->next->likedBy = (int *)malloc(sizeof(int));
    lastPost->next->likes = 0;
    lastPost->next->next = NULL;
    printf("\nyou just posted: %s\nwith postID:%d\n", lastPost->next->text, lastPost->next->postID);
}

/*****************************************************************
likes a post selected by user; makes sure user can't like it again
******************************************************************/
void like(PROFILE *profileHead, int activeUser, char *username, char *postID)
{
    PROFILE *currNode = profileHead;
    PROFILE *prevNode = profileHead;
    PROFILE *selectedProfile;
    /*checking if username exists*/
    int profileFound = 0;
    while (currNode != NULL)
    {
        if (strcmp(currNode->userName, username) == 0)
        {
            profileFound = 1;
            selectedProfile = currNode;
        }
        prevNode = currNode;
        currNode = currNode->next;
    }
    if (profileFound == 0)
    {
        printf("\nuser not found!\n");
        return;
    }
    POST *curr = selectedProfile->postHead;
    POST *prev = selectedProfile->postHead;
    int postFound = 0;
    while (curr != NULL)
    {
        int temp = atoi(postID);
        if (temp == curr->postID)
        {
            postFound = 1;
            int i = 0;
            for (i = 0; i <= curr->likes - 1; i++)
            {
                if (curr->likedBy[i] == activeUser)
                {
                    printf("\nYou can't like a post more than once!\n");
                    return;
                }
            }
            curr->likes++;
            curr->likedBy[curr->likes - 1] = activeUser;
            /*activeUser is equal to the profileID of the user who liked this*/
            curr->likedBy = (int *)realloc(curr->likedBy, (curr->likes + 1) * sizeof(int));
            printf("\nthe number of likes is: %d\n", curr->likes);
            printf("\nYou liked post number %d from user %s.\n", curr->postID, username);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    if (postFound == 0)
    {
        printf("\npost not found!\n");
        return;
    }
}

/*******************************************
deletes a post chose by user
********************************************/
void delete(PROFILE *profileHead, int activeUser, char *postID)
{
    /*since postID is a string, we need to read it as an integer*/
    int postNumber = atoi(postID);
    PROFILE *currProf = profileHead;
    PROFILE *prevProf = profileHead;
    PROFILE *activeProfile;
    while (currProf != NULL)
    {
        if (currProf->profileID == activeUser)
        {
            activeProfile = currProf;
            break;
        }
        prevProf = currProf;
        currProf = currProf->next;
    }
    POST *currNode = activeProfile->postHead;
    POST *prevNode = activeProfile->postHead;
    int postFound = 0;
    while (currNode != NULL)
    {
        if (currNode->postID == postNumber)
        {
            postFound = 1;
            /*freeing the memory*/
            free(currNode->text);
            free(currNode->user);
            prevNode->next = currNode->next;
            free(currNode);
            printf("\nPost was deleted successfully!\n");
            return;
        }
        prevNode = currNode;
        currNode = currNode->next;
    }
    if (postFound == 0)
    {
        printf("\nPost not found!\n");
    }
    return;
}

/********************************************
logs user out :)
*********************************************/
void logout(int *activeUSer, int *AlreadyInFunction)
{
    *activeUSer = 0;
    printf("logout successful\n");
    *AlreadyInFunction = 0;
    return;
}

/*********************************************
prints every single post of the active user
**********************************************/
void info(PROFILE *profileHead, int activeUser, int alreadyInFunction)
{
    if (alreadyInFunction == 0)
    {
        printf("\nYou need to log in first!\n");
        return;
    }
    PROFILE *currNode = profileHead;
    PROFILE *prevNode = profileHead;
    PROFILE *activeProf;
    while (currNode != NULL)
    {
        if (currNode->profileID == activeUser)
        {
            activeProf = currNode;
            break;
        }
        prevNode = currNode;
        currNode = currNode->next;
    }
    POST *currPost = activeProf->postHead->next;
    POST *prevPost = activeProf->postHead->next;
    /*printing all the posts*/
    printf("\n");
    printf("Username: %s\nPassword: %s\n\n", activeProf->userName, activeProf->password);
    while (currPost != NULL)
    {
        printf("Post: %s\nPostID: %d\nLikes: %d\n\n\n", currPost->text, currPost->postID, currPost->likes);
        prevPost = currPost;
        currPost = currPost->next;
    }
    return;
}

/***************************************************************************
attemps to find a user with the same userName as what the active user enters
****************************************************************************/
void find_user(PROFILE *profileHead, char *userName, int activeUser)
{
    int userFound = 0;
    PROFILE *currNode = profileHead;
    PROFILE *prevNode = profileHead;
    PROFILE *selectedProf;
    while (currNode != NULL)
    {
        if (strcmp(userName, currNode->userName) == 0)
        {
            userFound = 1;
            selectedProf = currNode;
            break;
        }
        prevNode = currNode;
        currNode = currNode->next;
    }
    if (selectedProf->profileID == activeUser)
    {
        printf("\nThis is YOUR username!\nIf you want to see your own information, please use the 'info' function.\n");
        return;
    }
    POST *currPost = selectedProf->postHead->next;
    POST *prevPost = selectedProf->postHead->next;
    /*printing the posts*/
    printf("\n");
    printf("Username: %s\n\n", selectedProf->userName);
    while (currPost != NULL)
    {
        printf("Post: %s\nPostID: %d\nLikes: %d\n\n\n", currPost->text, currPost->postID, currPost->likes);
        prevPost = currPost;
        currPost = currPost->next;
    }
    if (userFound == 0)
    {
        printf("\nUser not found! Try again.\n");
        return;
    }
    return;
}

void writeIntoFile(PROFILE *profileHead, FILE *accounts, FILE *posts)
{
    /*writing the info of the accounts into the file*/
    PROFILE *currNode = profileHead->next;
    PROFILE *prevNode = profileHead->next;
    POST *curr = profileHead->next->postHead;
    POST *prev = profileHead->next->postHead;
    accounts = fopen("accounts.txt", "w");
    posts = fopen("posts.txt", "w");
    while (currNode != NULL)
    {
        fputs(currNode->userName, accounts);
        fprintf(accounts, " ");
        fputs(currNode->password, accounts);
        /*we need to reinitialize curr every time we want to enter the second while*/
        curr = currNode->postHead;
        while (curr != NULL)
        {
            /*wrinting the info of the posts into the file*/
            if (curr->postID != 0)
            {
                fputs(curr->text, posts);
                fprintf(posts, " ");
                fputs(currNode->userName, posts);
                fprintf(posts, " ");
                fprintf(posts, "%d", curr->likes);
                fprintf(posts, "\n");
            }
            prev = curr;
            curr = curr->next;
        }
        fprintf(accounts, " ");
        /*the last post's postID is the total number of the posts*/
        fprintf(accounts, "%d", prev->postID);
        fprintf(accounts, "\n");
        prevNode = currNode;
        currNode = currNode->next;
    }
    fclose(accounts);
    fclose(posts);
    return;
}