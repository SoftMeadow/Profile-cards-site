import React, { useState } from 'react';
import { useNavigate } from "react-router-dom";
import { TextField, Button, Box, Typography, Container } from '@mui/material';

function Login(){
    const [login, setLogin] = useState('');
    const [email, setEmail] = useState('');
    const navigate = useNavigate();
    
    const HandleLogin = (e) => {
        e.preventDefault();
        if (login && email) {     
            // fetch('http://localhost:3500/info', {
            //     method: 'POST',
            //     headers: { 'Content-Type': 'application/json' },
            //     body: JSON.stringify({ login, email }),
            //   })  
            //   .then(response => response.json())
            //   .then(user => console.log('User added:', user));
            navigate("/profiles");
        } 
    }

    return (
        <Container
            sx={{ 
                display: 'flex', 
                flexDirection: 'column', 
                justifyContent: 'center', 
                alignItems: 'center', 
                textAlign: 'center',
                height: '100vh',
                px: { xs: 2, sm: 0 }
            }}
        >
            <Box component="form" onSubmit={HandleLogin} sx={{ mt: 4, textAlign: 'center', width: '100%', maxWidth: 400 }}>
                <Typography variant="h1" gutterBottom>Login</Typography>
                <Typography variant="h2" gutterBottom>Please submit your login and email</Typography>
                <TextField
                    label="Login"
                    value={login}
                    onChange={(e) => setLogin(e.target.value)}
                    fullWidth
                    margin="normal"
                />
                <TextField
                    label="Email"
                    type="email"
                    value={email}
                    onChange={(e) => setEmail(e.target.value)}
                    fullWidth
                    margin="normal"
                />
                <Button type="submit" variant="contained" color="primary">
                    Submit
                </Button>
            </Box>
        </Container>
    );
}

export default Login;