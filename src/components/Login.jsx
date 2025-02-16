import React, { useState, useContext } from 'react';
import { useNavigate } from "react-router-dom";
import { TextField, Button, Box, Typography, Container } from '@mui/material';
import { UserContext } from './ProfileData';

function Login(){
    const [login, setLogin] = useState('');
    const [email, setEmail] = useState('');
    const navigate = useNavigate();
    const { addUser } = useContext(UserContext);
    
    const HandleLogin = (e) => {
        e.preventDefault();
        if (HandleNull()) {
            return;
        }
        if (login && email) {       
            addUser({ login, email });
            navigate("/profiles");
        } 
    }
    
    const HandleNull = () => {
        if (!login || !email) {
            alert('The login and email bars should not be null!');
            return true;
        }
        return false;
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