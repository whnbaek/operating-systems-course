import subprocess

def get_logged_in_users_count():
    try:
        # Execute the 'who' command to list logged-in users
        result = subprocess.run(['who'], stdout=subprocess.PIPE, text=True, check=True)
        # Split the output into lines, each line corresponds to a logged-in user
        users = result.stdout.strip().split('\n')
        return len(users)
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {e}")
        return 0

if __name__ == "__main__":
    user_count = get_logged_in_users_count()
    print(f"Number of logged-in users: {user_count}")
