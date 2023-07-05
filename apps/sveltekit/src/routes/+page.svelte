<script lang="ts">
  import { onDestroy } from 'svelte';

  let status = '';
  if (Notification.permission === 'default') {
    Notification.requestPermission().then(result => {
      status = result;
      if (result === 'granted') {
        new Notification('Test');
      }
    });
  }

  let form: HTMLFormElement;
  const connection = {
    endpoint: 'home.jet-black.xyz:32199',
    label: 'Device1',
    active: false,
    timeout: 0
  };

  // Handle adding more endpoints
  const handleSubbmit = () => {
    connection.active = true;
  };

  const sendNotification = (msg: string) => {
    if (Notification.permission !== 'granted')
      throw new Error('Notification permission not granted');
    new Notification(msg, { icon: 'logo.png', vibrate: [100, 50, 100] });
  };

  // Watch for events
  const interval = setInterval(async () => {
    const now = new Date().getTime();
    if (connection.timeout > now) return;
    if (!connection.active) return;

    try {
      const controller = new AbortController();
      const timeout = setTimeout(() => controller.abort(), 500);
      const res = await fetch(`http://${connection.endpoint}/status`, {
        signal: controller.signal
      });
      clearTimeout(timeout);

      if (!res.ok) throw new Error("Response didn't have ok status");
      const data = await res.json();
      switch (data.requiresAssistance) {
        case 'YES':
          connection.timeout = new Date().getTime() + 10000;
          sendNotification('Asssistance button pressed');
          break;
        case 'NO':
          // todo...
          break;
      }
    } catch (error) {
      sendNotification('Cannot connected');
      connection.active = false;
      console.error(error);
    }
  }, 1000);

  onDestroy(() => {
    clearInterval(interval);
  });
</script>

<div class="form-control flex-1 items-center justify-center gap-y-20">
  <span
    class="bg-base-100 rounded-md border-2 px-4 py-2 text-2xl font-extralight md:text-3xl"
    class:text-primary={connection.active}
    class:border-primary={connection.active}
    class:text-error={!connection.active}
    class:border-error={!connection.active}>
    {connection.active ? 'Connected' : 'Disconnected'}</span>
  <form
    bind:this={form}
    action=""
    class="form-control gap-y-4"
    on:submit|preventDefault={handleSubbmit}>
    <label for="ip" class="input-group">
      <span class="w-32 whitespace-nowrap">Endpoint label</span>
      <input
        bind:value={connection.label}
        type="text"
        name="label"
        id="ip"
        class="input input-primary"
        placeholder="Endpoint ip" />
    </label>
    <label for="ip" class="input-group">
      <span class="w-32 whitespace-nowrap">Endpoint</span>
      <input
        type="text"
        name="endpoint"
        id="ip"
        class="input input-primary"
        placeholder="Endpoint ip"
        bind:value={connection.endpoint} />
    </label>
    <button class="btn btn-primary btn-outline" type="submit">Connect</button>

    <button
      class="btn btn-primary btn-outline"
      type="button"
      on:click={() => {
        connection.active = false;
      }}>Disconnect</button>
  </form>
</div>
