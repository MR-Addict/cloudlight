async function handleChange(event) {
  const data = new FormData();
  data.append("light", event.target.checked ? "on" : "off");
  await fetch("/api/light", { method: "POST", body: data });
}

async function handleLoaded() {
  const res = await fetch("/api/light").then((res) => res.json());
  document.getElementById("checkbox").checked = res.light === "on";
}

handleLoaded();
