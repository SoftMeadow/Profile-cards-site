import React, { useState, useEffect } from 'react';
import { Container, Box, Typography, Card, CardContent, TextField } from '@mui/material';
import './Profiles.css';

const SearchBar = ({ setSearchQuery }) => (
  <form onSubmit={(e) => e.preventDefault()}>
    <TextField
      id="search-bar"
      className="text"
      onInput={(e) => setSearchQuery(e.target.value)}
      label="Enter login"
      variant="outlined"
      placeholder="Search..."
      size="small"
    />
  </form>
);

function Profiles() {
  const [searchQuery, setSearchQuery] = useState('');
  const [users, setUsers] = useState([]);

  useEffect(() => {
    fetchUsers(searchQuery);
  }, [searchQuery]);

  async function fetchUsers(query) {
    const url = `http://212.41.9.231:8001/api/users/?page=1&page_size=100&query=${encodeURIComponent(query)}`;
    try {
      const response = await fetch(url, {
        method: "GET",
        headers: { 'Content-Type': 'application/json' }
      });

      if (!response.ok) {
        throw new Error(`Response failed: ${response.status}`);
      }

      const json = await response.json();
      console.log("API Response:", json); // Проверка ответа API

      if (json.values && Array.isArray(json.values)) {
        setUsers(json.values);
      } else {
        console.warn("API response does not contain 'values' array:", json);
        setUsers([]);
      }
    } catch (error) {
      console.error("Error fetching users:", error.message);
      setUsers([]);
    }
  }

  return (
    <Container className="container">
      <SearchBar setSearchQuery={setSearchQuery} />
      <Box className="box">
        {users.length === 0 ? (
          <Typography variant="h5">No users found</Typography>
        ) : (
          users.map((user) => (
            <Card key={user.id} className="card">
              <CardContent>
                <Typography variant="h5">{user.fio || "No Name"}</Typography>
                <Typography variant="body1">{user.email || "No Email"}</Typography>
              </CardContent>
            </Card>
          ))
        )}
      </Box>
    </Container>
  );
}

export default Profiles;
