Author: Gheorghe Mara, 323 CD, Politehnica University of Bucharest

HTTP client implementation

INTRODUCTION

This program implements a client that communicates with a server using the HTTP protocol. 
The server has been implemented by the Communication Protocols team and it provides a basic movie library that supports multiple users. 
The client offers a wide array of commands ranging from user accounts management by the admin to each user being able to view and change their own movie libraries.
The progarm uses an open-source github project called "parson" to help handle JSON objects: "https://github.com/kgabis/parson.git"

HEADER FILES

    1. The helper.h header provides wrapper functions for communicating with the server, for handling the cookies list and for extracting helpful information out of a server response, such as an error or a JWT access token.

    2. The extra functions in client.h ensure that the input data respects the format it's supposed to have. There is however a MAX_INPUT_TRIES limit to how many times invalid input can be provided, after which the application stops. This limit can be adjusted in client.h

    3. In requests.h, before the command functions, it is worth pointing out:
        - the compute_X_request() functions which create a request of type X that is ready to be sent to the server.
        - post_json() that serves as a wrapper that executes compute_post_request then sendss the. data to the server, updates the cookies and displays either the extracted ERROR or a custom SUCCESS message 

COMMAND FUNCTIONS:

    1. login_admin (POST request)
        Authenticates an admin user. Prompts for username and password.
        Upon success, generates a cookie that must be used as authentification for all admin operations
        Output: 
            SUCCESS: Admin autentificat cu succes

    2. add_user (POST request)
        Adds a new normal user (admin only). Prompts for username and password.
        Output: 
            SUCCESS: Utilizator adăugat cu succes

    3. get_users (GET request)
        Fetches the list of all users (admin only).
        Output:
            SUCCESS: Lista utilizatorilor  
            #1 test:test  
            #2 john:doe

    4. delete_user (DELETE request)
        Deletes a normal user (admin only). Prompts for username.
        Output: 
            SUCCESS: Utilizator șters

    5. login (POST request)
        Authenticates a normal user. Prompts for admin username, username, and password.
        Upon success, generates a cookie associated to that user.
        Output: 
            SUCCESS: Autentificare reușită

    6. get_access (GET request)
        Requests access to the current user's movie collections.
        Upon success, generates a JWT token that is used for all operations on movies & collections
        Output: 
            SUCCESS: Token JWT primit

    7. get_movies (GET request)
        Fetches all movies from the server.
        Output:
            SUCCESS: Lista filmelor  
            #1 The Dark Knight  
            #2 Snow White

    8. get_movie (GET request)
        Fetches detailed information about a specific movie by id.
        Output:
            SUCCESS: Detalii film  
            title: The Dark Knight  
            year: 2010  
            description: sci-fi  
            rating: 8.8

    9. add_movie (POST request)
        Adds a new movie. Prompts for the title, year, description, and rating.
        Output: 
            SUCCESS: Film adăugat

    10. delete_movie (DELETE request)
        Deletes a movie by id.
        Output: 
                SUCCESS: Film șters cu succes

    11. update_movie (PUT request)
        Updates movie data by id. Prompts for the new title, year, description, and rating.
        Output: 
                SUCCESS: Film actualizat

    12. get_collections (GET request)
        Fetches all movie collections from the server.
        Output:
                SUCCESS: Lista colecțiilor
                #1: Colectie A
                #2: Colectie B

    13. get_collection (GET request)
        Fetches detailed information about a specific movie collection by id.
        Output:
                SUCCESS: Detalii colecție
                title: Favorite Movies
                owner: test_user
                #3: Inception
                #5: Interstellar

    14. add_collection (POST request)
        Adds a new movie collection. Prompts for title, number of movies, and movie ids.
        Output:
                SUCCESS: Colecție adăugată

    15. delete_collection (DELETE request)
        Deletes a movie collection by id.
        Output:
                SUCCESS: Colecție ștearsă

    16. add_movie_to_collection (POST request)
        Adds a movie to a collection. Prompts for collection_id and movie_id.
        Output:
                SUCCESS: Film adăugat în colecție

    17. delete_movie_from_collection (DELETE request)
        Removes a movie from a collection. Prompts for collection_id and movie_id.
        Output:
                SUCCESS: Film șters din colecție

    18. logout_admin (GET request)
        Logs out the currently authenticated admin.
        Output:
                SUCCESS: Admin delogat

    19. logout (GET request)
        Logs out the currently authenticated user.
        Output:
                SUCCESS: Utilizator delogat

    20. exit (closes the communciation with the server)
        Closes the program without displaying a message.

    * any other command will be met with an invalid command error and the user can try again

To run the program, simply run `make` in the source rooth directory, then `./client` and you are ready to input commands.
