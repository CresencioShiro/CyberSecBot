import sqlite3

def connect_to_database(db_name):
    """Connects to the specified SQLite database or creates one if it doesn't exist."""
    try:
        conn = sqlite3.connect(db_name)
        print(f"Connected to database {db_name}")
        return conn
    except sqlite3.Error as e:
        print(f"Error connecting to database: {e}")
        return None

def create_table(conn):
    """Creates a users table with columns for user ID, username, and password."""
    try:
        query = """
        CREATE TABLE IF NOT EXISTS users (
            user_id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL,
            password TEXT NOT NULL
        );
        """
        conn.execute(query)
        conn.commit()
        print("Users table created successfully.")
    except sqlite3.Error as e:
        print(f"Error creating table: {e}")

def add_user(conn, username, password):
    """Inserts a new user into the users table using a parameterized query."""
    try:
        query = "INSERT INTO users (username, password) VALUES (?, ?);"
        conn.execute(query, (username, password))
        conn.commit()
        print(f"User '{username}' added successfully.")
    except sqlite3.Error as e:
        print(f"Error adding user: {e}")

def get_user(conn, username):
    """Fetches user details securely using a parameterized query."""
    try:
        query = "SELECT * FROM users WHERE username = ?;"
        cursor = conn.execute(query, (username,))
        result = cursor.fetchone()
        if result:
            print(f"User found: ID={result[0]}, Username={result[1]}, Password={result[2]}")
        else:
            print("User not found.")
    except sqlite3.Error as e:
        print(f"Error fetching user: {e}")

def main():
    # Connect to the database
    conn = connect_to_database("secure_app.db")

    if conn:
        # Create the users table
        create_table(conn)

        # Add a user securely
        add_user(conn, "john_doe", "securepassword123")

        # Attempt to retrieve a user securely
        get_user(conn, "john_doe")

        # Close the connection when done
        conn.close()
        print("Connection closed.")

if __name__ == "__main__":
    main()
