async function handleChange(event) {
  await fetch("/api/led", { method: "POST" });
}

async function handleLoaded() {
  const res = await fetch("/api/led").then((res) => res.json());
  document.getElementById("toggle").checked = res.led === "off";
}

handleLoaded();
