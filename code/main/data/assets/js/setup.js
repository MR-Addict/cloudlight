async function handleSubmit(event) {
  event.preventDefault();
  try {
    await fetch("/api/credentials", { body: new FormData(event.target), method: "POST" });
    document.body.innerHTML = "<p>设备重启中...</p>";
    await fetch("/api/reboot", { method: "POST" });
  } catch (err) {
    console.error(err);
  }
}

async function handleLoaded() {
  const res = await fetch("/api/credentials").then((res) => res.json());
  document.getElementById("ssid").value = res.ssid;
  document.getElementById("password").value = res.password;
}

handleLoaded();
