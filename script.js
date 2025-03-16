const output = document.getElementById('output');
const commandInput = document.getElementById('command');

function executeCommand(command) {
  if (command === 'help') {
    appendOutput('Available commands:\n- help: Show this help message\n- play: Play connect 4\n- hello: Say hello\n- clear: Clear the console');
  } else if (command === 'hello') {
    appendOutput('Hello, user!');
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
