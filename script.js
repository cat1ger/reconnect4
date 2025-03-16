let gameLoaded = false;

// Initialize WebAssembly Module
Module.onRuntimeInitialized = () => {
  gameLoaded = true;
  console.log('Game module loaded!');
};

// Intercept standard output
Module.print = (text) => {
  appendOutput(text);
};

// Optional: Intercept standard error (e.g., for debugging)
Module.printErr = (text) => {
  appendOutput(`[ERROR] ${text}`);
};

// Function to append output to the webpage
function appendOutput(text) {
  const outputDiv = document.getElementById('output');
  const line = document.createElement('div');
  line.textContent = text;
  outputDiv.appendChild(line);
}

function executeCommand(command) {
  if (command === 'play') {
    if (gameLoaded) {
      Module._main(); // Call the C program's main function
    } else {
      appendOutput('Game is still loading. Please wait...');
    }
  } else if (command === 'help') {
    appendOutput('Available commands:\n- play: Start the game\n- help: Show help\n- clear: Clear the console');
  } else if (command === 'clear') {
    output.innerHTML = '';
  } else {
    appendOutput(`Unknown command: ${command}`);
  }
}

function appendOutput(text) {
  const line = document.createElement('div');
  line.textContent = text;
  output.appendChild(line);
}

commandInput.addEventListener('keypress', (event) => {
  if (event.key === 'Enter') {
    const command = commandInput.value.trim();
    executeCommand(command);
    commandInput.value = ''; // Clear the input
  }
});
