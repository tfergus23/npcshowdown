const express = require('express');
const path = require('path');
const app = express();

// Serve the 'public' directory as static files
app.use(express.static('.'));

app.get('*', (req ,res) => {
    const filePath = path.join(__dirname, 'index.html'); 
    res.sendFile(filePath);
});

// Start the server
app.listen(4200, () => {
  console.log('Server listening on port 4200');
});
