import express from 'express';
import path from 'path';
import { htmlString } from './App';

const port = 3000;
const app = express();

app.route('/').get((_req, res) => {
  res.send(htmlString);
});

app.use(express.static(__dirname));
app.use(express.static(path.resolve( `${__dirname}/../assets`)));

app.listen(port, () => {
  console.log(`App listening on http://localhost:${port}`);
});
