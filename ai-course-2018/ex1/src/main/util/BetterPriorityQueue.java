package main.util;


import java.util.PriorityQueue;

public class BetterPriorityQueue<E> {
    private PriorityQueue<QueueElement<E>> queue;

    /**
     * Ctor
     */
    public BetterPriorityQueue() {
        // comparator for the priority queue compares between the priorities of each state
        this.queue = new PriorityQueue<>((e1, e2) -> (int) (e1.getPriority() - e2.getPriority()));
    }

    /**
     * Inserts an element to the queue with a given priority
     * @param elem - the element
     * @param priority - the priority of that element
     * @return boolean signifes the success of the operation
     */
    public boolean put(E elem, double priority) {
        QueueElement<E> queueElem = new QueueElement<>(elem, priority);
        return this.queue.add(queueElem);
    }

    /**
     * Pulls out an element from the priority queue
     * @return - the element that was pulled out. has min priority
     */
    public E get() {
        QueueElement<E> queueElem = this.queue.poll();
        return queueElem.getObj();
    }

    // returns whether the queue contains the given item.
    public boolean contains(E elem) {
        QueueElement<E> queueElem = new QueueElement<>(elem, 0);
        return this.queue.contains(queueElem);
    }

    // self explanatory
    public boolean isEmpty(){
        return this.queue.isEmpty();
    }

    // tuple of element and priority, so I could keep track of the priority
    // of each item.
    private class QueueElement<E> {
        private E obj;
        private double priority;

        public QueueElement(E obj, double priority) {
            this.obj = obj;
            this.priority = priority;
        }

        public E getObj() {
            return this.obj;
        }

        public double getPriority() {
            return this.priority;
        }

        @Override
        public int hashCode() {
            return this.obj.hashCode();
        }

        @Override
        public boolean equals(Object other) {
            return this.obj.equals(other);
        }
    }
}
