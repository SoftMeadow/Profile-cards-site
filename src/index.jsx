import React from 'react';
import { createRoot } from 'react-dom/client';
import { ThemeProvider, createTheme } from '@mui/material/styles';
import App from './App';
import './index.css';
import { UserProvider } from './components/ProfileData'

const theme = createTheme({
  typography: {
      fontFamily: '"Rubik", serif',
      h1: {
          fontSize: 40,
          fontWeight: 500,
          fontFamily: '"Rubik", serif;',
      },
      h2: {
        fontSize: 20,
        fontWeight: 100,
        fontFamily: '"Rubik", serif;',
      },
      h3: {
        fontSize: 30,
        fontWeight: 200,
        fontFamily: '"Rubik", serif;',
      },
      body1: {
        fontSize: 16,
        fontWeight: 300,
        fontFamily: '"Rubik", serif;',
      }
  },
  palette: {
    primary: { main: '#4b6ea5' },
    secondary: { main: '#ce93d8' }
  },
});

const root = createRoot(document.getElementById('root'));

root.render(
<React.StrictMode>
  <ThemeProvider theme={theme}>
    <UserProvider>
      <App />
    </UserProvider>
  </ThemeProvider>
</React.StrictMode>
);
