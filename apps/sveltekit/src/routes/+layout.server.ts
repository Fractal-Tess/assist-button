import type { LayoutServerLoad } from './$types';

export const ssr = false;
export const prerender = true;

export const load = (({ locals: { theme } }) => {
  return {
    theme
  };
}) satisfies LayoutServerLoad;
