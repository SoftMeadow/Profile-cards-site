import { createContext, useState } from 'react';

export const UserContext = createContext();

export const UserProvider = ({ children }) => {
    const [users, setUsers] = useState([
            { login: 'user1', email: 'user1@example.com' },
            { login: 'user2', email: 'user2@example.com' },
            { login: 'user3', email: 'user3@example.com' }
    ]);

    const addUser = (user) => {
        setUsers([...users, user]);
    };

    return (
        <UserContext.Provider value={{ users, addUser }}>
            {children}
        </UserContext.Provider>
    );
};
