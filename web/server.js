const express = require("express");
const shelljs = require("shelljs");
const path = require("path");
const fs = require("fs");
const cors = require("cors");

const app = express();

app.use(express.json());
app.use(cors());
app.use(express.static("public"));

app.set("view engine", "ejs");
app.set("views", path.join(__dirname, "views"));

app.post("/compile", (req, res) => {
  try {
    const inputData = req.body.code;
    if (!inputData) {
      res.status(400).json({ msg: "Digite o código primeiro." });
      return;
    }

    fs.writeFileSync("./in.txt", inputData);

    const { code: exitCode } = shelljs.exec("./compiler/tiny < in.txt > out.txt");

    if (exitCode != 0) {
      res.status(400).json({ msg: "Não foi possível compilar o código!" });
      return;
    }

    const outputData = fs.readFileSync("./out.txt").toString();
    console.log(outputData);

    res.status(200).json({ data: outputData });
  } catch (error) {
    res.status(500).json({ msg: error });
  }
});

app.get("/", (req, res) => {
  res.render("index", {
    apiUrl: process.env.API_URL,
  });
});

app.listen(process.env.PORT);
