cd ~/xon

# Check current remote URL
git remote -v

# Update to new username
git remote set-url origin https://github.com/isaacmuliro/Xerxis-Object-Notation-.git

# Verify the change
git remote -v

# Now pull the remote changes
git pull origin main --rebase

# Then push your changes
git push origin main

<!-- OR FORCE IT -->
git push origin main --force