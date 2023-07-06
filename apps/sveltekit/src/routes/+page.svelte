<script lang="ts">
  import { onDestroy } from 'svelte';

  if (Notification.permission === 'default') {
    Notification.requestPermission().then(result => {
      if (result === 'granted') {
        new Notification('Test');
      }
    });
  }

  let form: HTMLFormElement;
  const connection = {
    endpoint: 'https://assist-button-proxy.app.jet-black.xyz/status',
    label: 'Device1',
    active: false,
    timeout: 0,
    connected: false
  };

  // Handle adding more endpoints
  const handleSubmit = () => {
    connection.active = true;
  };

  const sendNotification = (msg: string) => {
    if (Notification.permission !== 'granted')
      throw new Error('Notification permission not granted');
    new Notification(msg, { icon: 'logo.png', vibrate: [100, 50, 100] });
  };

  const sleep = (time: number) =>
    new Promise(resolve => setTimeout(resolve, time));
  // Watch for events

  (async () => {
    for (;;) {
      // Throttle
      await sleep(1000);

      // Check if the `active` status
      if (!connection.active) {
        connection.connected = false;
        continue;
      }

      try {
        // Give 500 ms for fetching
        const controller = new AbortController();
        const timeout = setTimeout(() => controller.abort(), 500);
        const res = await fetch(connection.endpoint, {
          signal: controller.signal
        });
        clearTimeout(timeout);
        connection.connected = true;

        // Throw on non-200
        if (!res.ok) throw new Error("Response didn't have ok status");

        // Parse
        const data = await res.json();
        switch (data.requiresAssistance) {
          case 'YES':
            // Send notification and wait 10 s
            sendNotification('Assistance button pressed');
            await sleep(10_000);
            break;
          case 'NO':
            // NOOP
            break;
        }
      } catch (error) {
        // If previously was connected
        if (connection.connected) sendNotification('Cannot connected');
        // Signal disconnection
        connection.connected = false;
        console.error(error);
      }
    }
  })();
</script>

<div class="form-control flex-1 items-center justify-center gap-y-20">
  <span
    class="bg-base-100 rounded-md border-2 px-4 py-2 text-2xl font-extralight md:text-3xl"
    class:text-primary={connection.connected}
    class:border-primary={connection.connected}
    class:text-error={!connection.connected}
    class:border-error={!connection.connected}>
    {connection.connected ? 'Connected' : 'Disconnected'}</span>
  <form
    bind:this={form}
    action=""
    class="form-control gap-y-4"
    on:submit|preventDefault={handleSubmit}>
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
