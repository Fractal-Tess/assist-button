<script lang="ts">
  import ThemeToggle from '$components/theme/ThemeToggle.svelte';
  import { faGithub } from '@fortawesome/free-brands-svg-icons';
  import Fa from 'svelte-fa';
</script>

<div class="flex h-20 items-center justify-end px-4">
  <div
    class="[&>*:hover]:text-primary flex items-center space-x-4 [&>*]:transition-all [&>*]:duration-300">
    <a
      href="https://github.com/Fractal-Tess/SvelteKit"
      rel="noreferrer"
      target="_blank">
      <Fa icon={faGithub} class="text-3xl" />
    </a>
    <ThemeToggle />
  </div>
</div>
