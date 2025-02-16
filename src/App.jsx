import { BrowserRouter, Routes, Route } from 'react-router-dom';
import Login from './components/Login';
import Profiles from './components/Profiles';

function App(){
    return(
        <BrowserRouter>
            <Routes>
                <Route path="/" element={<Login />} />
                <Route path="login" element={<Login />} />
                <Route path="profiles" element={<Profiles />} />
            </Routes>
        </BrowserRouter>
    )
}

export default App;

