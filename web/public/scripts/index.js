const textarea = document.getElementById("inputCompiler");

textarea.addEventListener("keydown", e => {
  if (e.key == "Tab") {
    e.preventDefault();

    textarea.setRangeText("  ", textarea.selectionStart, textarea.selectionStart, "end");
  }
});

async function compile() {
  try {
    const inputData = document.getElementById("inputCompiler").value;

    const response = await fetch(`${API_URL}/compile`, {
      method: "POST",
      headers: {
        Accept: "application/json",
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ code: inputData }),
    });

    const outputCompilerError = document.getElementById("outputCompilerError");
    const outputCompiler = document.getElementById("outputCompiler");

    if (!response.ok) {
      const { msg } = await response.json();
      outputCompilerError.innerText = msg;
      outputCompiler.innerText = "";

      outputCompiler.classList.add("d-none");
      outputCompilerError.classList.remove("d-none");
      return;
    }

    const { data } = await response.json();

    outputCompiler.innerText = data;
    outputCompilerError.innerText = "";

    outputCompiler.classList.remove("d-none");
    outputCompilerError.classList.add("d-none");
  } catch (error) {
    console.error(error);
  }
}
