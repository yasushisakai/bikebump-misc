import { createReadStream, createWriteStream, readFile, writeFile } from 'fs'
import { resolve } from 'path'

function readFilePromise (filePath: string): Promise<string []> {
  return new Promise((resolve, reject) => {
    readFile(filePath, (error, buffer: Buffer) => {
      if (error !== null) {
        return reject(error)
      }
      resolve(buffer.toString().split('\n'))
    })
  })
}

function excludeHidden (dirname: string): boolean {
  return !(dirname.startsWith('.') || dirname.startsWith('random'))
}

function bellFrequencyMap (bellName: string): number {
  switch (bellName) {
    case 'gold' :
      return 2484
    case 'oi' :
      return 2765
    case 'silver' :
      return 2390
    case 'hubway' :
      return 2953
    case 'crane' :
      return 3609
    default :
      return 3000
  }
}

function didItMatch (line: string[]): boolean {
  // console.log(line)
  let count = 0
  for (const element of line) {
    count++
    if (element.trim() === 'didPass') {
      return line[count].trim() === '1'
    }
  }
  return false
}

function getTotalDuration (line: string[]): number {
  const index = line.indexOf('detectionWhole')
  if (index !== -1) {
    const tempDuration = parseInt(line[index + 1], 10)
    if (tempDuration  < 0) {
      const start = parseInt(line[line.indexOf('waiting') + 1], 10)
      const end   = parseInt(line[line.indexOf('leaving') - 1], 10)
      return end - start
    } else {
      return tempDuration
    }
  } else {
    return null
  }
}

function checkCase (baseObj: any, line: string[], key: string, blacklist: string[] = []): void {
  for (const black of blacklist) {
    if (black === line[0]) {
      return
    }
  }

  if (!(key in baseObj)) {
    baseObj[key] = [0, 0]
  }
  baseObj[key][0] ++

  if (didItMatch(line)) {
    baseObj[key][1] ++
  }
}

async function main () {
  console.log(process.argv[2])

  const fileTag: string = process.argv[2] ? process.argv[2] : 'res0'
  const csvFilePath: string = resolve ( __dirname, `../../OFSoundAnalyser/bin/data/out_${fileTag}.csv`)

  const csvData: string[] = await readFilePromise(csvFilePath)

  // console.log(csvData)

  // X. check for bad recordings
  const recordings: { [key: string]: number[]} = {}
  const csvDataByElements: string[][] = [] // just to make it easier afterwards
  for (const line of csvData) {
    const elements: string[] = line.split(',').map((element) => element.trim())
    csvDataByElements.push(elements)
    checkCase (recordings, elements, `${elements[0]}`)
  }

  // console.log(recordings)

  const blacklist: string[] = []

  // manual blacklist
  blacklist.push('crane_1497215857813_s_0_1_3609.wav')
  blacklist.push('crane_1497215562000_s_1_0_3609.wav')

  blacklist.push('gold_1497211445342_n_0_1_2484.wav')
  blacklist.push('gold_1497215954942_s_1_0_2484.wav')

  blacklist.push('hubway_1497214178700_n_0_1_2953.wav')
  blacklist.push('hubway_1497213988839_n_1_0_2953.wav')

  blacklist.push('oi_1497212216024_n_1_0_2765.wav')
  blacklist.push('oi_1497216125723_s_1_0_2765.wav')

  blacklist.push('random_1497213359095_n_0_0_2953.wav')
  blacklist.push('random_1497213256527_n_0_0_2390.wav')

  blacklist.push('silver_1497211913824_n_0_1_2390.wav')
  blacklist.push('silver_1497211879161_n_1_0_2390.wav')

  blacklist.push('silver_1497211913824_n_0_1_2390.wav')
  blacklist.push('silver_1497211879161_n_1_0_2390.wav')

  blacklist.push('')

  Object.keys(recordings).map((key: string) => {
    const recordingInfo: string[] = key.split('_')

    // putting double dings and random to blacklist
    if (!(recordingInfo[3] === '1') || key.startsWith('random')) {
    // putting single dings and random to blacklist
    // if ((recordingInfo[4] !== '1') || key.startsWith('random')) {
      blacklist.push(key)
    }
  })

  // how many ms did we need to detect one ding?
  // get the average ding detection time for a single ding
  let averageDetectDuration = 0
  let maxDetectDuration = 0
  let minDetectDuration = 1000000
  let detectCount = 0
  for (const line of csvDataByElements) {
    if (didItMatch(line)) {
      if (blacklist.indexOf(line[0]) === -1) {
        continue
      }
      const tempDuration = getTotalDuration(line)
      if (tempDuration !== null) {
        maxDetectDuration = Math.max(maxDetectDuration, tempDuration)
        minDetectDuration = Math.min(minDetectDuration, tempDuration)
        averageDetectDuration += tempDuration
        detectCount ++
      }
    }
  }

  console.log(`${averageDetectDuration} / ${detectCount} = ${averageDetectDuration / detectCount} `)

  console.log(`min: ${minDetectDuration}, max: ${maxDetectDuration}`)

  // console.log(blacklist)

  // A. which threshold is the best?
  const thresholds: { [key: string]: number[] } = {}

  for (const line of csvDataByElements) {
    checkCase (thresholds, line, `${line[1]}`, blacklist)
  }

  console.log(thresholds)

  // B. which duration holds the best?
  const durations: {[key: string]: [number]} = {}

  for (const line of csvDataByElements) {
    checkCase (durations, line, `${line[2]}`, blacklist)
  }

  console.log(durations)

  // C. A., B. combined
  const combined: { [thresholdAndDurations: string]: number[] } = {}

  for (const line of csvDataByElements) {
    const key: string = `t${line[1]}d${line[2]}`
    checkCase (combined, line, key, blacklist)
  }

  const ratio: { [key: string]: number } = {}
  Object.keys(combined).map((key: string) => {
    ratio[key] = combined[key][1] / combined[key][0]
  })

  console.log(ratio)

  // D. nessary detection length

  const result: any = {
    blacklist,
    thresholds,
    durations,
    combined: ratio,
  }

  const resultJSON: string = JSON.stringify(result)
  /*
  writeFile(resolve(__dirname, `./out/out_${fileTag}_${Date.now()}.json`), resultJSON, (error) => {
    if (error) {
      return console.error(error)
    }

    console.log('file written')
  })
  */
}

main()
