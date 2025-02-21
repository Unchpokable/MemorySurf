<script lang="ts">
    import { onMount } from 'svelte';
    import { invoke } from '@tauri-apps/api/core';
    import { fly } from 'svelte/transition';
    import { FullDump, Match } from '../lib/fulldump';

    let content: any | null;
    let fullDump: FullDump = new FullDump();

    const matchIcons = [
        { type: Match.NoMatch, name: 'noMatch' },
        { type: Match.Match7, name: 'match7' },
        { type: Match.Match11, name: 'match11' },
        { type: Match.Match21, name: 'match21' }
    ];

    $: totalTraffic = fullDump.totalTraffic;
    $: collectedTraffic = fullDump.collectedTraffic;
    $: skillRating = fullDump.skillRating;

    $: hasSeeingRed = fullDump.hasSR;
    $: hasButterNinja = fullDump.hasBN;
    $: maxMatch = fullDump.maxMatch;
    $: hasCleanFinish = fullDump.hasCF;

    let initial = {
        'score': 0,
        'statsArray': [
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0
        ],
        'largestMatch': 0,
        'timeElapsed': 0
    }

    async function getArgs(): Promise<Record<string, string>> {
        return await invoke("get_cli_args");
    }

    function update() {
        if(content != null) {
            console.log("going to update dump!");
            fullDump = FullDump.fromRaw(content);
        }
    }

    function onSocketMessage(event : MessageEvent) {
        content = event.data;
        update();
    }

    function onSocketOpen(event : Event) {
        content = initial;
    }

    function onSocketClose(event : CloseEvent) {
        content = initial;
    }

    function onSocketError(event : Event) {
        content = initial;
    }

    onMount(() => {
        let webSocket: WebSocket | null = null;

        async function setupWebSocket() {
            try {
                const args: Record<string, string> = await getArgs();
                const ws = args.ws;
                if (!ws) {
                    console.error('Параметр ws не найден');
                    return;
                }
                webSocket = new WebSocket(`ws://localhost:${ws}`);
                webSocket.addEventListener('message', onSocketMessage);
                webSocket.addEventListener('open', onSocketOpen);
                webSocket.addEventListener('close', onSocketClose);
                webSocket.addEventListener('error', onSocketError);
            } catch (error) {
                console.error('Ошибка при получении аргументов:', error);
            }
        }
        setupWebSocket();

        content = initial;
        update();
        console.log("updating");

        return () => {
            if (webSocket) {
                webSocket.close();
            }
        };
    });

</script>

<div class="content">
    <p class="sr_title" transition:fly={{y: 20, duration: 100}}> Skill Rating (if CF): { Math.trunc(skillRating) }</p>
    <table>
        <tbody>
            <tr>
                {#each totalTraffic as value, i (i)}
                <td>
                    <div class='stat_item' transition:fly={{ y: 20, duration: 100}}> 
                        {value}
                    </div>
                </td>
                {/each}
            </tr>
            <tr>
                {#each collectedTraffic as value, i (i)}
                  <td>
                    <div class='stat_item' transition:fly={{ y: 20, duration: 400 }}>
                      {value}
                    </div>
                  </td>
                {/each}
              </tr>
        </tbody>
    </table>

    <div class='horizontal_container'>
        {#if hasButterNinja[0]}
            <img class='icon' src='bn.svg' alt='a gde'/>
        {:else}
            <img class='icon' src='bn_unacquired.svg' alt='a gde'/>
        {/if}

        {#if hasSeeingRed[0]}
            <img class='icon' src='sr.svg' alt='a gde'/>
        {:else}
            <img class='icon' src='sr_unacquired.svg' alt='a gde'/>
        {/if}

        {#each matchIcons as icon}
        <img
            src={maxMatch[0] === icon.type
            ? `/icons/${icon.name}-active.png`
            : `/icons/${icon.name}-inactive.png`}
            alt={`Иконка ${icon.name}`}
        />
        {/each}
    </div>
</div>

<style>
    @font-face{
        font-family: "Exo-Bold";
        src: url("/fonts/ExoSoft-Bold.ttf");
        font-weight: normal;
        font-style: normal;
    }
    @font-face{
        font-family: "Exo-SemiBold";
        src: url("/fonts/ExoSoft-SemiBold.ttf");
        font-weight: normal;
        font-style: normal;
    }
    @font-face{
        font-family: "Exo-Normal";
        src: url("/fonts/ExoSoft-Regular.ttf");
        font-weight: normal;
        font-style: normal;
    }

    :global(body) {
        margin: 0;
        padding: 30px;
        color: #dad1c3;
        font-size: 36;
    }

    div {
        display: flex;
        flex-direction: column;
        align-items: center;      /* Центрирует по вертикали */
        justify-content: center;  /* Центрирует по горизонтали */
        width: 100%;              /* Занимает всю ширину родительского элемента */
        height: 100%;             /* Занимает всю высоту родительского элемента */
    }

    .content {
        font-family: "Exo-Bold";
        font-size: 36px;
        background: linear-gradient(-45deg, #160631, #4d1616);
        border-radius: 10px;
    }

    div.horizontal_container {
        flex-direction: row;
    }

    .sr_title {
        font-size: 36px;
    }

    table {
        border-collapse: collapse;
        margin: 10px 0;
        table-layout: auto;
        width: 100%;
    }

    td {
        border: 0px transparent;
        padding: 8px 12px;
    }
</style>
