import React, { useState, useEffect } from 'react';
import { Container, Box, Typography, Card, CardContent, TextField, Button } from '@mui/material';
import './Profiles.css';

const SearchBar = ({ setSearchQuery }) => (
  <form onSubmit={(e) => e.preventDefault()}>
    <TextField
      id="search-bar"
      className="text"
      onInput={(e) => {
        setSearchQuery(e.target.value);
      }}
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
    fetch('http://localhost:3500/info')
      .then(response => response.json())
      .then(setUsers);
  }, []);


  const filterData = (query, users) => {
    if (!query) {
      return users;
    }
    return users.filter((user) => user.login.toLowerCase().includes(query.toLowerCase()));
  };

  const filteredUsers = filterData(searchQuery, users);

  return (
    <Container className="container">
      <SearchBar setSearchQuery={setSearchQuery} />
      <Box className="box">
        {filteredUsers.map((user, index) => (
          <Card key={index} className="card">
            <CardContent>
              <Typography variant="h3">{user.login}</Typography>
              <Typography variant="body1">{user.email}</Typography>
            </CardContent>
          </Card>
        ))}
      </Box>
    </Container>
  );
}

export default Profiles;